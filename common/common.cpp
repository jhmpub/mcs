#include <winsock2.h>
#include <windows.h>  // after winsock2 to prevent including winsock.h
#include <ctype.h>
#include <stdio.h>
#include "common.h"
#include "socket.h"
#include "tira_dll.h"
#include "tira_tx.c"

// tira dll function pointers
tira_init_t              tira_init;
tira_cleanup_t           tira_cleanup;
tira_set_handler_t       tira_set_handler;
tira_start_t             tira_start;
tira_stop_t              tira_stop;
tira_start_capture_t     tira_start_capture;
tira_cancel_capture_t    tira_cancel_capture;
tira_get_captured_data_t tira_get_captured_data;
tira_transmit_t          tira_transmit;
tira_delete_t            tira_delete;

// initializing to the maximum volume levels insures sufficient "down"
// volume commands will be issued during ampInit to reach volume floors
int masterVolume=MASTER_VOLUME_MAX;
int centerVolume=EFFECT_VOLUME_MAX;
int subwooferVolume=EFFECT_VOLUME_MAX;
int surroundRightVolume=EFFECT_VOLUME_MAX;
int surroundLeftVolume=EFFECT_VOLUME_MAX;

int audioPowerState;
int audioSource=VIDEO_AUX;
int exiting=FALSE;
int hdmiSwitchState=UNDEFINED;
int initializing=FALSE;
int muteState=MUTE_OFF;
int registrationCount=0;
int station=FM1;
int surroundProfile=SURROUND_STD;
int undefined=UNDEFINED;

static int force_exit=FALSE;
static volatile const char * lock=NULL;

// speaker state
int bSpeakerState=CHANNEL_B_OFF;
int center_speaker_relay=CENTER_SPEAKER_OFF;
int channel_a_director=CHANNEL_A_TO_FRONT_SPEAKERS;
int channel_a_relay=CHANNEL_A_OFF;
int currentEffectSpeaker=0;
int frontSpeakerState=FRONT_SPEAKERS_OFF; 
int rear_speaker_selector=REAR_SPEAKERS_FROM_CHANNEL_A;
int rearSpeakerState=REAR_SPEAKERS_OFF;
int subwoofer_relay=SUBWOOFER_OFF;
int subwooferState, LFE_State;


HMODULE tiraDLL = NULL;

struct serverState serverState, *preserved;


void setState(const char * szMsg) {

   struct tiraCmd * tiraCmd = tiraCmdSzToTiraCmd(szMsg);
   int cmd = tiraCmd->id;
   
   char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
   
   switch (cmd) {
      case MUTE_TOGGLE:
         muteState = (cmd==MUTE_OFF) ?  MUTE_ON : MUTE_OFF;
         break;
      default:
         if (tiraCmd->state!=&undefined)
            *tiraCmd->state = volumeCmd ? getVolumeLevel(szMsg) : cmd;
         break;
   }
}


int getVolumeLevel(const char * msg) {

   const char * vol, * level=NULL;

   if ((vol=strstr(msg, SZ_VOLUME)) &&
            strstr(msg, SZ_VOLUME_UP)==NULL &&
            strstr(msg, SZ_VOLUME_DOWN)==NULL) {
      level=vol+strlen(SZ_VOLUME);    
   }
   
   return level ? atoi(level) : 0;
}   


BOOL isValidCmdId(int cmd) {

   int i;
   
   for (i=0; i<sizeof(tiraCmd)/sizeof(struct tiraCmd); i++) {
       if (cmd==tiraCmd[i].id)
           return TRUE;
   }    
   return FALSE;
}


BOOL isTiraCmdStateSet(int cmd) {
   int i;
   for (i=0; i<sizeof(tiraCmd)/sizeof(struct tiraCmd); i++) {
      if (cmd==tiraCmd[i].id && cmd==*(tiraCmd[i].state))
          return TRUE;
   }
         
   return FALSE;
}


int tiraCmdSzToId(const char * msg) {

   int i;
   char * msgBuffer = (char *) malloc(strlen(msg)+1); 
   char * vol;
   strcpy(msgBuffer,msg);
   
   // truncate the absolute volume number
   if ((vol=strstr(msgBuffer, SZ_VOLUME)) &&
            strstr(msgBuffer, SZ_VOLUME_UP)==NULL &&
            strstr(msgBuffer, SZ_VOLUME_DOWN)==NULL) {
      *(vol+strlen(SZ_VOLUME)) = '\0';    
   }    
   
   for (i=0; i<sizeof(tiraCmd)/sizeof(struct tiraCmd); i++) {
      if ( !(strcmp(msgBuffer, tiraCmd[i].sz)) )
         return tiraCmd[i].id;
   }   

   free(msgBuffer);
   return UNDEFINED;
}            


const char * tiraCmdIdToSz(int cmd) {

   static char szVolume[256], szFormat[256];
   char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
   struct tiraCmd * tiraCmd = tiraCmdIdToTiraCmd(cmd);
   const char * retval = SZ_UNDEFINED;
   
   if (volumeCmd) {
      // append the volume string with its absolute volume level
      char * vol;
      const char * volPattern = SZ_VOLUME;
      strcpy(szFormat, tiraCmd->sz);
      if (vol=strstr(szFormat, volPattern)) {
         *(vol+strlen(volPattern)) = 0;
         strcat(szFormat, "%d");
         sprintf(szVolume, szFormat, *(tiraCmd->state));
         retval=szVolume;
      }
   } else {
      retval = tiraCmd->sz;
   }    
   
   return retval;
}


struct tiraCmd * tiraCmdSzToTiraCmd(const char * szCmd) {
   return tiraCmdIdToTiraCmd(tiraCmdSzToId(szCmd));
}


struct tiraCmd * tiraCmdIdToTiraCmd(int cmd) {

   int i;
   
   for (i=0; i<sizeof(tiraCmd)/sizeof(struct tiraCmd); i++) {
      if (tiraCmd[i].id==cmd) {
         return &tiraCmd[i];
      }   
   }
   
   printf("tiraCmdIdToTiraCmd: no translation for the \"%d\" command\n", cmd);
   
   return &tiraCmd[UNDEFINED];
}


int ttyLoop() {
   char c = '\0';
   
   Sleep(500);   // pause to let console messages print out
   help();
   tty_buffering(FALSE);
   
   while (c!='q') {
      c=getChar();
      
      switch (c) {
         case '\n':
            break;
         case 'q':
            if (force_exit || exiting) {
               tty_buffering(TRUE);
               exit(0);
            } else {
               force_exit=TRUE;
               processClientMsg(SZ_EXIT_PROCESS);
               c='\0';
            }   
            break;
         default:
            help();
      }
   }
   return 0;
}


void help() {
   printf( "\ntype \'q\' to quit\n");
}


void errorExit (const char * szMessage) { 
   fprintf(stderr, "\n%s\n", szMessage); 
   ExitProcess(GetLastError()); 
}


HANDLE getStdinHandle() {
   char szErrMsg[256];
   HANDLE hStdin;

   if ((hStdin=GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE) {
      snprintf(szErrMsg, sizeof(szErrMsg), 
               "getStdinHandle: error %d", GetLastError());
      errorExit(szErrMsg); 
   }
   return hStdin;
}   


#ifdef MCS
// MCS functions specifically defined for media control system agents

void updateState(const char * szMsg) {
   setState(szMsg);
   broadcastMsg(szMsg);
}

void processClientMsg(const char * msg) { msgQueueAdd(msg); }
void processClientMsg(const char * msg, struct socketDescriptor * sd) {
   int cmd = tiraCmdSzToId(msg);
   
   switch (cmd) {
      case REGISTER:
      case REREGISTER:
         sd->registered=TRUE;
         break;
      case DEREGISTER:
         sd->registered=FALSE;
         break;
   }

   // queue client messages to synchronize multithreaded processing
   // must expose socketDescriptor for multithreaded message context
   msgQueueAdd(msg, sd);
}

void processClientDisconnect(struct socketDescriptor * sd) { 
   if (sd->registered && sd->connected) {
       // registered and connection still alive
       deregisterClient(sd); 
   }    
   sd->registered = FALSE;   
}


// function to start the thread for processing TCP/IP messages
BOOL isClientMsgThreadRunning=FALSE;
void processClientMsgThreadStart() {
   HANDLE hThread;
   if (hThread=CreateThread(
         NULL,    // default security
         0,       // default stack size
         (LPTHREAD_START_ROUTINE) processClientMsgThread,
         NULL,    // thread parameter
         0,       // run thread immediately
         NULL)    // ignore thread id
      ) {
      CloseHandle(hThread);
      isClientMsgThreadRunning=TRUE;
   } else {
      printf("processClientMsgThreadStart failed\n");
   }   
}

#else 
void processClientMsg(const char * msg) {}
void processClientDisconnect() {}
const char * tira_dll_absolute_path = 
   "\\\\georgia\\backup\\install\\drivers\\interface\\usb\\tira\\sdk\\Tira2.dll";
#endif   // #ifdef MCS


int tiraTransmit(struct tiraCmd * cmd) {
   int result;
   static const unsigned char * irCodes = NULL;
   
   if (!irCodes)
      irCodes = (const unsigned char *) GlobalAlloc(GPTR, 1024);
      
   memcpy((void *)irCodes, cmd->codes, cmd->size);
   result = tira_transmit(0, -1, irCodes, cmd->size);
   
   if (result)
      printf("tiraTransmit: tira_transmit of the %s cmd failed. errno %d\n", cmd->sz, result);
   
   return result;
}      


// initialize the HOMElectronics Tira2 IR receiver/transmitter
int initTiraRemoteControl(int port) {
   int error;
   
   loadTiraDll();
   printf("Tira library loaded\n");
   
   if (tira_init()) {
      printf("initTiraRemoteControlPort: tira_init failed\n");
      exit(1);
   }

   printf("Tira activat%s on com port %d\n", 
         (error=tira_start(port-1)) ? 
         "ion failed" : "ed", port);
         
   return error;
}   


void loadTiraDll() {

   const char * dll;
   
   const DWORD SZLEN = 256;
   TCHAR tira_dll_cwd[SZLEN];
   GetCurrentDirectory(SZLEN, tira_dll_cwd);
   strcat(tira_dll_cwd,"\\");
   strcat(tira_dll_cwd, TIRA_DLL);
   
   // Search for the tira2 dll to run as a service or in console mode in case
   // the dll is not in the current directory.  Unfortunately, running rcclient
   // as a service bars SendInput from interacting with the desktop which makes
   // synthesized keystrokes impossible
   if (!access(tira_dll_cwd, F_OK))
      dll = tira_dll_cwd;
   else if (!access(tira_dll_absolute_path, F_OK))
      dll = tira_dll_absolute_path;
   else 
      dll = TIRA_DLL;
   
   if ( !(tiraDLL=LoadLibrary(dll)) )
      printLoadTiraDllErrorAndExit("LoadLibrary(Tira2.dll)");
      
   if ( !(tira_init = (tira_init_t) GetProcAddress(tiraDLL , "tira_init")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_init)");
       
   if ( !(tira_cleanup = (tira_cleanup_t) GetProcAddress(tiraDLL , "tira_cleanup")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_cleanup)");
       
   if ( !(tira_set_handler = (tira_set_handler_t) GetProcAddress(tiraDLL , "tira_set_handler")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_handler)");
       
   if ( !(tira_start = (tira_start_t) GetProcAddress(tiraDLL , "tira_start")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_start)");
       
   if ( !(tira_stop = (tira_stop_t) GetProcAddress(tiraDLL , "tira_stop")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_stop)");
       
   if ( !(tira_start_capture = (tira_start_capture_t) GetProcAddress(tiraDLL , "tira_start_capture")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_start_capture)");
       
   if ( !(tira_cancel_capture = (tira_cancel_capture_t) GetProcAddress(tiraDLL , "tira_cancel_capture")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_cancel_capture)");
       
   if ( !(tira_get_captured_data = (tira_get_captured_data_t) GetProcAddress(tiraDLL , "tira_get_captured_data")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_get_captured_data)");
       
   if ( !(tira_transmit = (tira_transmit_t) GetProcAddress(tiraDLL , "tira_transmit")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_transmit)");
       
   if ( !(tira_delete = (tira_delete_t) GetProcAddress(tiraDLL , "tira_delete")) )
       printLoadTiraDllErrorAndExit("GetProcAddress(tira_delete)");
}


void unloadTiraDll() {
   if (tiraDLL) {
      tira_cleanup();
      FreeLibrary(tiraDLL);
      tiraDLL = NULL;
   }   
}


void createProcess(const char * exeFile, const char * arg) {
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   memset(&si,0,sizeof(si));
   si.cb = sizeof(si);
   memset(&pi,0,sizeof(pi));
   
   char * moduleDir = (char *) malloc(strlen(exeFile)+1);
   char * slash;
   strcpy(moduleDir,exeFile);
   if(slash=strrchr(moduleDir,'\\'))
      *slash=0;
      
   char * cmdLine = (char *) malloc(strlen(exeFile)+strlen(arg)+2);
   strcpy(cmdLine,exeFile);
   strcat(cmdLine," ");
   strcat(cmdLine,arg);
   
   if ( !CreateProcess( 
          NULL,                // executable file (white space supported)
          cmdLine,             // exe with arguments (no white space in exe argument)
          NULL,                // Process handle not inheritable
          NULL,                // Thread handle not inheritable
          FALSE,               // Set handle inheritance to FALSE
          DETACHED_PROCESS,    // creation flags
          NULL,                // Use parent's environment block
          moduleDir,           // current directory for the process
          &si,                 // Pointer to STARTUPINFO structure
          &pi ))               // Pointer to PROCESS_INFORMATION structure
   {
      printf( "CreateProcess failed (%d).\n", GetLastError() );
   }
   free(moduleDir);
   free(cmdLine);
} 


char * szToLower(char * sz) {
   char *p=sz; 
   for (; *p; ++p) *p=tolower(*p);
   return sz;
}


// local atomic compare and swap 
// if lock == FALSE, lock = TRUE; return FALSE
// if lock == TRUE, lock = TRUE; return TRUE
LONG isLocked(volatile LONG * lock) {
   return InterlockedCompareExchange(lock, TRUE, FALSE);
}

// local atomic unlock
void unlock(volatile LONG * lock) { *lock=FALSE; }


// global atomic compare and swap
// if lock == NULL, lock = szFunc; return FALSE
// if lock != NULL, return TRUE
int isLocked(const char * szFunc) {
   if (InterlockedCompareExchange((volatile LONG *) &lock, lock!=NULL, FALSE))
      return TRUE;
   else {
      lock=szFunc;
      //printf("%s has lock\n", szFunc);
      return FALSE;
   }
}

// global atomic unlock
void unlock() { lock=NULL; }

// findWindow is a WinAPI FindWindow replacement that finds the first
// window with a case insenstive version of the given string and then 
// returns the corresponding HWND or NULL 
HWND findWindow(const char * sz) {
   int i, titleLen = 0;
   char windowTxt[256];
   char * title = (char *) malloc(strlen(sz) + 1);
   szToLower(strcpy(title, sz));
   HWND hWnd = GetTopWindow(NULL);
   while(hWnd) {
      hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
      if (GetWindowText(hWnd, windowTxt, sizeof(windowTxt)) && 
          strstr(szToLower(windowTxt), title))
         return hWnd;
   }
   return NULL;
}


//// begin general purpose functions

BOOL isConsoleMode() {
#ifdef CONSOLE_MODE
   return TRUE;
#else   
   return FALSE;
#endif
}   


#ifdef __CYGWIN__
// CYGWIN is more posix compliant so it works better with other posix compliant
// shells like bash.  The downside is termio stuff wont compile with MSVC or MingW.
// Added here for debugging flexibility

#include <unistd.h>
#include <termios.h>

void tty_buffering(int enable) {

   struct termios tio;
   
   tcgetattr(STDIN_FILENO, &tio);
   enable ? tio.c_lflag |= (ICANON | ECHO) : tio.c_lflag &= (~ICANON & ~ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &tio);
}   

// arrow keys come as an escaped sequence:
// 0x1b
// 0x5b
// CHAR_ARROW_<direction>
#define GETCHAR_ESCAPE_SEQ1  0x1b
#define GETCHAR_ESCAPE_SEQ2  0x5b
#define GETCHAR_UP           0x41
#define GETCHAR_DOWN         0x42
#define GETCHAR_RIGHT        0x43
#define GETCHAR_LEFT         0x44

char getChar(void) {

   char c;
   int seq=0;           
   
   while (TRUE) { 
   
      c=getchar();
      
      switch (c) {
         case GETCHAR_ESCAPE_SEQ2:
            if (!seq)
               return c;
         case GETCHAR_ESCAPE_SEQ1:
            seq++;
            break;
         default:
            if (seq==2) {
               switch (c) {
                  case GETCHAR_UP:
                     return CHAR_ARROW_UP;
                     break;
                  case GETCHAR_DOWN:
                     return CHAR_ARROW_DOWN;
                     break;
                  case GETCHAR_RIGHT:
                     return CHAR_ARROW_LEFT;
                  break;
                  case GETCHAR_LEFT:
                     return CHAR_ARROW_RIGHT;
                     break;
                  default:
                     continue;
               }
            }   
            else if (isprint(c)) {
               return tolower(c);
            }   
      }
   }
}               

char * keyToName(char keyChar, char * szName, int size) {

   switch (keyChar) {
      case '\r': 
         strncpy(szName, "cr", size);
         break;
      case CHAR_ARROW_UP:
         strncpy(szName, "up", size);
         break;   
      case CHAR_ARROW_DOWN:
         strncpy(szName, "dn", size);
         break;   
      case CHAR_ARROW_RIGHT:
         strncpy(szName, "rt", size);
         break;   
      case CHAR_ARROW_LEFT:
         strncpy(szName, "lt", size);
         break;
      default:      
         snprintf(szName, size, "%c", keyChar);
   }
         
   return szName;
   
}

#else
 
// disabling and enabling winapi line mode buffering via GetConsoleMode
// and SetConsoleMode in place of the tcgetattr and tcsetattr functions 
// is not needed when using ReadConsoleInput in place of getchar
void tty_buffering(int enable) {}

char getChar(void) {
   HANDLE hStdin = getStdinHandle(); 
   DWORD irCount;
   INPUT_RECORD irBuf[1];
   char szErrMsg[256];

   while (TRUE) { 
      if (!ReadConsoleInput(hStdin, irBuf, 
                            sizeof(irBuf)/sizeof(INPUT_RECORD), &irCount) ) {
         snprintf(szErrMsg, sizeof(szErrMsg), "ReadConsoleInput: error %d", GetLastError());
         errorExit(szErrMsg); 
      }

      KEY_EVENT_RECORD ker = irBuf[0].Event.KeyEvent;
      if (ker.bKeyDown) {
         // return keyboard down events and
         // no modifier key events (ctrl, shift, alt, etc)
         if (ker.uChar.AsciiChar == CHAR_ARROW_UP ||
             ker.uChar.AsciiChar == CHAR_ARROW_DOWN || 
             ker.uChar.AsciiChar == CHAR_ARROW_RIGHT || 
             ker.uChar.AsciiChar == CHAR_ARROW_LEFT) {
            // skip remapped keyboard events
            // ctrl-X, ctrl-Y, ctrl-Z, ctrl-[
            continue;
         }
         else if (ker.wVirtualKeyCode == VK_UP)
            return CHAR_ARROW_UP;
         else if (ker.wVirtualKeyCode == VK_DOWN)
            return CHAR_ARROW_DOWN;
         else if (ker.wVirtualKeyCode == VK_RIGHT)
            return CHAR_ARROW_RIGHT;
         else if (ker.wVirtualKeyCode == VK_LEFT)
            return CHAR_ARROW_LEFT;
         else if (ker.uChar.AsciiChar)
            return tolower(ker.uChar.AsciiChar);
      }   
   }      
}

char * keyToName(char keyChar, char * szName, int size) {
   
   switch (keyChar) {
      case '\r': 
         strncpy(szName, "cr", size);
         break;
      default:      
         snprintf(szName, size, "%c", keyChar);
   }      
   
   return szName;
}

#endif // CYGWIN

static LinkedList * printQueue;        // prevents jumbled console logs 
static volatile LONG printQueueLock;   // insure exclusive thread access
void printQueueAdd(char * sz) { 
   while (isLocked(&printQueueLock))   
      Sleep(10);
   szQueueAdd(&printQueue, sz); 
   unlock(&printQueueLock);
}
char * printQueueRemove() { 
   while (isLocked(&printQueueLock))   
      Sleep(10);
   char * sz = szQueueRemove(&printQueue); 
   unlock(&printQueueLock);
   return sz;
} 


static LinkedList * msgQueue;        // serialize message processing
static volatile LONG msgQueueLock;   // insure exclusive thread access

BOOL isMsgQueueEmpty() { 
   while (isLocked(&msgQueueLock))   
      Sleep(10);
   BOOL isEmpty = isQueueEmpty(msgQueue); 
   unlock(&msgQueueLock);
   return isEmpty;
}

void msgQueueAdd(const char * szMsg) { 
   QueuedMsg * msg = new QueuedMsg(szMsg);
   while (isLocked(&msgQueueLock))   
      Sleep(10);
   queueAdd(&msgQueue, msg); 
   unlock(&msgQueueLock);
}

void msgQueueAdd(const char * szMsg, IrCmd * irCmd) { 
   QueuedMsg * msg = new QueuedMsg(szMsg, irCmd);
   while (isLocked(&msgQueueLock))   
      Sleep(10);
   queueAdd(&msgQueue, msg); 
   unlock(&msgQueueLock);
}

void msgQueueAdd(const char * szMsg, struct socketDescriptor * sd) { 
   QueuedMsg * msg = new QueuedMsg(szMsg, sd);
   while (isLocked(&msgQueueLock))   
      Sleep(10);
   queueAdd(&msgQueue, msg); 
   unlock(&msgQueueLock);
}

QueuedMsg * msgQueueRemove() { 
   while (isLocked(&msgQueueLock))   
      Sleep(10);
   QueuedMsg * queuedMsg = (QueuedMsg *) queueRemove(&msgQueue);
   unlock(&msgQueueLock);
   return queuedMsg;
}   


// generic routine to add a string to a queue
void szQueueAdd(LinkedList ** queuePtr, const char * sz) {
   char * qEntry = (char *) malloc(strlen(sz)+1);
   strcpy(qEntry, sz);
   queueAdd(queuePtr, (void *) qEntry); 
}

// generic routine to remove a string from a queue
// returns NULL if queue is empty
char * szQueueRemove(LinkedList ** queuePtr) { 
   return (char *) queueRemove(queuePtr); 
}


// add an entry onto the end of the given LinkedList
void queueAdd(LinkedList ** queuePtr, void * ptr) {
   BOOL isEmpty=TRUE;
   
   if (!*queuePtr)
      *queuePtr = new LinkedList();
      
   LinkedList * q = *queuePtr;   
   
   for (q=q->first(); q->isNotLast(); q=q->next()) {
      if (!isEmpty && q->get() == NULL) {
         // reuse an existing entry
         q->add(ptr);
         return;
      }   
      if (q->get() != NULL)
         isEmpty=FALSE;
   }
   
   if (isEmpty) {
      // queue is empty, add as first 
      q->first()->add(ptr);
   }   
   else {
      // queue is full, add onto the end   
      q->add(ptr);
   }
   
   return;
}         

// remove and return an entry from the beginning of the given LinkedList
// return NULL if the queue is empty
void * queueRemove(LinkedList ** queuePtr) { 
   void * retval=NULL;
   
   if (!*queuePtr)
      *queuePtr = new LinkedList();
      
   LinkedList * q = *queuePtr;   

   for (q=q->first(); q->isNotLast(); q=q->next()) {
      if (q->get() != NULL) {
         retval = q->get();
         q->set(NULL);
         break;
      }   
   }
   return retval;
}

BOOL isQueueEmpty(LinkedList * q) {
   if (q==NULL)
      return TRUE;

   for (q=q->first(); q->isNotLast(); q=q->next()) {
      if (q->get() != NULL)
         return FALSE;
   }
   
   return TRUE;
}


HANDLE initExitHandler() {
   HANDLE hThread = CreateThread(NULL, 
                                 0, 
                                 (LPTHREAD_START_ROUTINE)dispatchThread,
                                 0, 0, 0);    
   if (!hThread) {
      printf("initExitHandler: CreateThread failed with errno %d", 
          GetLastError());                              
      exit(1);
   }   
    
   return hThread;       
}


void dispatchThread() {
   MSG msg;
    
    // window created is invisible window without 
    // WS_VISIBLE ORd into the 4th argument
   HWND myDialog = CreateWindowEx(
      0,WC_DIALOG,"rcClient Dispatch Thread",WS_OVERLAPPEDWINDOW,
      400,100,200,200,NULL,NULL,NULL,NULL
   );
    
   SetWindowLong(myDialog, DWL_DLGPROC, (long)exitHandler);
   
   while(GetMessage(&msg,NULL,0,0))  {
      TranslateMessage(&msg);
      DispatchMessage(&msg); 
   } 
}


// handle a "net stop <service>" request
LRESULT WINAPI exitHandler(HWND hwnd, 
                           UINT msg, 
                           WPARAM wParam, 
                           LPARAM lParam) {
    switch(msg) {
       case WM_ENDSESSION:
       case WM_CLOSE:
       case WM_QUERYENDSESSION:
          processClientMsg(SZ_EXIT_PROCESS);
          PostQuitMessage(0);
          break;
       default:   
        return DefWindowProc(hwnd, msg, wParam, lParam); 
     }   
   return 0; 
}                               


void initConsoleModeExitHandler() {
   if( !SetConsoleCtrlHandler(
       (PHANDLER_ROUTINE) consoleModeExitHandler, TRUE) ) {
       printf("Error: SetConsoleCtrlHandler call failed with error code %d\n",
              GetLastError());
       exit(1);
   }         
}   


// handle a system shutdown or reboot event
BOOL consoleModeExitHandler(DWORD event) { 
   switch(event) { 
     case CTRL_C_EVENT: 
        printf("received a ctrl-c signal\n");
     case CTRL_CLOSE_EVENT: 
     case CTRL_SHUTDOWN_EVENT: 
        if (force_exit || exiting) {
           tty_buffering(TRUE);
           exit(0);
        } else {
           force_exit=TRUE;
           processClientMsg(SZ_EXIT_PROCESS);
        }   
        return TRUE;
 
     // Pass other signals to the next handler. 
     case CTRL_BREAK_EVENT: 
     case CTRL_LOGOFF_EVENT: 
        return FALSE; 
 
     default: 
        return FALSE; 
   } 
}


void printLoadTiraDllErrorAndExit(const char * msg) {
   printf("Error: loadTiraDll %s failed\n", msg);
   ExitProcess(1);
}    


int debug=FALSE;
void processArgs(int argc, char ** argv) {
   int i, startupDelay, comPort=0;

   for (i=0;i<argc;i++) {
   
      if (!strcmp(argv[i],"-d"))
         debug=TRUE;
      else if(!strcmp(argv[i],"-delay"));
      else if (i && !strcmp(argv[i-1],"-delay")) {
         // delay startup during boot to allow dependencies to initialize
         if ( (startupDelay=atoi(argv[i])) && 
              ((GetTickCount() < 60000) || isConsoleMode()) ) {
            printf("%d second startup delay...", startupDelay);
            fflush(stdout);
            Sleep(startupDelay * 1000);
            printf("done\n");
         }   
      } 
      else if (i) {
         usage(argv[0]);
         exit(0);
      }   
   }
}   


void usage(char * exe) {
   // extract the exe file basename from cygwin or mingw run time libraries
   char * exeFilename = strrchr(exe, '/') ? strrchr(exe, '/') + 1 : exe;
   exeFilename = strrchr(exeFilename, '\\') ? strrchr(exeFilename, '\\') + 1 : exeFilename;
   char * exeExt = strrchr(exeFilename, '.') ? strrchr(exeFilename, '.') : exe + strlen(exe);
   *exeExt = '\0';
   printf("\nUsage: %s [-d] [-delay <seconds>]\n", exeFilename);
}


