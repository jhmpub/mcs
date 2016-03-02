// This program runs as a background process that accepts commands from a 
// universal remote.  If a translation is found for an IR command, this process
// will either forward the command as an IP datagram to the Amplifier Control 
// Server or synthesize pressing a shortcut key to the process that has keyboard 
// focus 
//
// Universal Remote Translation Table
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// IR Cmd           Result
// All Modes
// Volume UP     -> Amplifier Volume Up
// Volume Down   -> Amplifier Volume Down
// Mute          -> Amplifier Mute

// TV Mode
// TV Power      -> Toggle Amplifier On; Init Matrix; Init Volume; Set TV Input/Amplfier Off
// TV/Video      -> Toggle TV/DVD/LVR_MM Input
// Menu Right    -> Surround Sound On
// Menu Left     -> Surround Sound Off
// Play          -> WMC - synthesize pressing Enter
// Pause         -> WMC - synthesize pressing Ctrl-P
// <<            -> WMC - synthesize pressing Ctrl-B
// >>            -> WMC - synthesize pressing Ctrl-F
// Stop          -> WMC - synthesize pressing Alt-F4
//
// VCR Mode
// Play          -> Videolan - synthesize pressing Enter
// Pause         -> Videolan - synthesize pressing space
// <<            -> Videolan - synthesize pressing Alt-Left Arrow
// >>            -> Videolan - synthesize pressing Alt-Right Arrow
// Menu Right    -> Videolan - synthesize pressing Shift-N
// Menu Left     -> Videolan - synthesize pressing Shift-P
// Stop          -> Videolan - synthesize pressing Alt-F4
//
// Notes:
//
// Running as a Service
// Windows Vista and later will not synthesize keyboard input if this
// application is run as a service because it violates the security model.
// The rcClient application is intended to be run from the startup folder.
// The rcClientConsole application is intended to be used for debugging.
//
// Build Information
// Build the targets with the build.bat script in the directory with this file.
// The rcClient target is built to run as a detached windows background
// process during startup.  The -mno-cygwin -mwindows options compile 
// and link using MinGW.  The MinGW option comes with a Cygwin installation 
// if all the development tools are installed. The rcClientConsole target 
// is used for debugging.
//
// 2008 Sep 10 jhm original creation
// 2009 Apr 10 jhm added IR cmd translations for videolan 
//                 media player
// 2013 Apr 28 jhm added IR cmd translations for windows media center
// 2015 Dec 24 jhm added a thread to listen for mute and audio source notifications
//

// SendInput() is only declared for Windows XP and later
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <unistd.h>
#include "../../../common/tira.h"
#include "../../../common/xantech_codes.h"
#include "../../../common/yamaha_codes.h"
#include "../../../common/common.h"
#include "../../../common/extern.h"
#include "rcClient.h"

int main(int argc, char ** argv) {

   int error;
   char * szPort = processArgs(argc, argv);
   int port = atoi(szPort) ? atoi(szPort) : DEFAULT_COM_PORT;
   
   loadTiraDll();
   printf("Tira library loaded\n");
   tira_init();
   initDefaults();
   initWinsock();
   
   printf("Tira activat%s on com port %d\n", 
          (error=tira_start(port-1)) ? "ion failed" : "ed", port);
   if (error) { printf(usage); exit(1); }
   
   printf("Callback capture activat%s\n", 
           (error=tira_set_handler(irCmdCallback)) ? "ion failed" : "ed");
   if (error) exit(1);
   
   HANDLE hThread = initExitHandler();
   
   initRxBroadcastThread();
   
   isConsoleMode() ?
      ttyLoop() :
      WaitForSingleObject(hThread,INFINITE);
}


void ttyLoop() {
   char c=0;
   
   help();
   stty_unbuffered();
   
   while (1) {
      if (c!='\xFF')
         printf(">");
      c=getchar();
      if (c!='\xFF' && c!='\n')
         printf("\n");
      
      switch (c) {
         case '\xFF':
            // tty apparently does not block in unbuffered mode
            Sleep(1000);
            break;
         case '\n':
            break;
         case 'q':
         case 'Q':
            stty_buffered();   // default back to flushing after a LF
            WSACleanup();      // release winsock
            printf("Exiting...\n");
            exit(0);
         default:
            help();
      }
   } 
}


void initRxBroadcastThread() {
   if (CreateThread(NULL, // default security
                    0,    // default stack size
                    (LPTHREAD_START_ROUTINE)&rxBroadcastThread,
                    NULL, // no thread parameter
                    0,    // run thread immediately
                    NULL) // ignore thread id
        == NULL) {
      printf("Error creating rxBroadcastThread");
      exit(1);
   }
} 


DWORD rxBroadcastThread(LPVOID param) {  
   //
   // blocking loop to receive broadcast updates from the audio control server
   //
   int nLen = sizeof(SOCKADDR);
   int nRet, cmdId;
   char msgBuf[256];
   SOCKADDR saClient;

   while(1) {
      memset(msgBuf, 0, sizeof(msgBuf));
      nRet = recvfrom(rxSocket,          // Bound socket
                      msgBuf,            // Receive buffer
                      sizeof(msgBuf),    // Size of buffer in bytes
                      0,                 // Flags
                      &saClient,         // Buffer to receive client address 
                      &nLen);            // Length of client address buffer
                   
      if (nRet == SOCKET_ERROR) {
         if(WSAGetLastError()==WSAEINTR) {
            // application exit interrupted the blocking recvfrom call
            ExitThread(0);
         }   
         PRINTERROR("rxBroadcastThread recvfrom");
      }
      
      if ( (cmdId=filterCmd(cmdSzToId(msgBuf))) != UNDEFINED ) {
         updateState(cmdId);
         //printf("Data received: %s\n", msgBuf);
      }   
   }   
}


// update state information received from audio control server broadcast notifications
void updateState(int cmdId) {

   BOOL printMsg = FALSE;

   switch(cmdId) {
      case MUTE_ON:
      case MUTE_OFF:
         if (muteState!=cmdId)
            printMsg=TRUE;
         muteState=cmdId;
         break;
      case FIRST_REGISTRATION:
         if (tvPowered) {
            printMsg=TRUE;
            setSurroundSound(displayState==DVD);
            sendCmd(SZ_FRONT_SPEAKERS_ON);
            sendCmd(szDefaultMasterVolume);
            setAudioSource(displayState);
         }   
         break;
      case GEORGIA:
      case JIMSON:
      case LVR_MM:
      case DEN_MM:
      case FM:
      case TV:
      case DVD:
         if (audioSource!=cmdId)
            printMsg=TRUE;
         audioSource=cmdId;
         break;
      default:
         printMsg=TRUE;
   }
   
   if (printMsg)
      printf("updateState: %s\n", cmdIdToSz(cmdId));
   
}         
 
    
void initWinsock()
{
   WORD wVersionRequested = MAKEWORD(1,1);
   WSADATA wsaData;
   int nRet;

   //
   // Initialize WinSock and check the version
   //
   WSAStartup(wVersionRequested, &wsaData);
   if (wsaData.wVersion != wVersionRequested) {
      PRINTERROR("initWinsock winsock version");
      exit(1);
   }
   
   //
   // Initialize the rxSocket recvfrom parameter used by rxBroadcastThread
   //
   rxSocket = socket(AF_INET,       // IPV4
                     SOCK_DGRAM,    // winsock UDP
                     IPPROTO_UDP);  // IP header value
   if (rxSocket==INVALID_SOCKET) {
        PRINTERROR("initWinsock create rx socket");
        exit(1);
   }
   // bind the socket to an IPV4 port on all host interfaces 
   SOCKADDR_IN rcClient;
   rcClient.sin_family = AF_INET;
   rcClient.sin_addr.s_addr = INADDR_ANY; // receive packets on any interface
   rcClient.sin_port = htons(AUDIO_REMOTE_CONTROL_NOTIFICATION_PORT);

   nRet = bind(rxSocket,                   // Socket descriptor
               (LPSOCKADDR)&rcClient,      // Address to bind to
               sizeof(struct sockaddr)     // Size of address
               );
   if (nRet==SOCKET_ERROR) {
       PRINTERROR("initWinsock bind rx socket");
       closesocket(rxSocket);
       exit(1);
   }
   
   //
   // Initialize the txSocket and acServer sendto parameters used by sendCmd
   //
   txSocket = socket(AF_INET,       // IPV4
                     SOCK_DGRAM,    // winsock UDP
                     IPPROTO_UDP);  // IP header value
   if (txSocket == INVALID_SOCKET) {
      PRINTERROR("initWinsock: create tx socket");
      exit(1);
   }

   // specify IPV4 and the audio control server address and port info
   LPHOSTENT lpHostEntry;
   lpHostEntry = gethostbyname(AUDIO_CONTROL_SERVER_HOSTNAME);
   if (lpHostEntry == NULL) {
      PRINTERROR("initWinsock gethostbyname");
      exit(1);
   }
   
   acServer.sin_family = AF_INET;
   acServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
   acServer.sin_port = htons(AUDIO_CONTROL_REQUEST_PORT);
}


void initDefaults() {
   if (!strlen(szDefaultMasterVolume))
      sprintf(szDefaultMasterVolume, "%s%s", SZ_MASTER_VOLUME,
                                             SZ_DEFAULT_MASTER_VOLUME);
   displayState=TV;
   audioSource=UNDEFINED;
   muteState=MUTE_OFF;
   tvPowered=FALSE;
}                                             


void sendCmd(char * szCmd) {

   //
   // send a cmd to the server
   //
   int nRet;
   printf("sendCmd: \"%s\"\n", szCmd);

   nRet = sendto(txSocket,                 // Socket
                 szCmd,                    // Data buffer
                 strlen(szCmd),            // Length of data
                 0,                        // Flags
                 (LPSOCKADDR)&acServer,    // Server address
                 sizeof(struct sockaddr)); // Length of address
              
   if (nRet == SOCKET_ERROR)
      PRINTERROR("sendCmd sendto");
      
   Sleep(100);   // pause to avoid an overrun   
      
   return;
}


struct translation * lookup(char const * capturedCmd) {

   int i,j,k;
   char const * tableCode;
   
   for (i=0; i<sizeof(translation)/sizeof(struct translation); i++) {
      for(j=0; j<translation[i].size; j++) {
         tableCode = translation[i].cmdCodes[j];
         //printf("%s: %s\n", translation[i].description, tableCode);
         for (k=0;
              k<CMD_CODE_LENGTH &&
              capturedCmd[k]==tableCode[k]; k++);
         
         if (k==CMD_CODE_LENGTH) {
             printf("\nmatch found: %s->%s\n", capturedCmd, translation[i].description);
             return &translation[i];
         }    
      }    
   }
   
   printf("Notice: capturedCmd msg match not found for %s\n", capturedCmd);
   
   return &translation[0];
}                


void stty_unbuffered() {
#ifdef CONSOLE_MODE
   // get a copy of the stdin settings and disable the buffering flag
   tcgetattr(STDIN_FILENO, &old_tio);
   new_tio=old_tio;
   new_tio.c_lflag &=(~ICANON);
   tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
#endif   
}


void stty_buffered() {
#ifdef CONSOLE_MODE
   // restore the former stdin settings
   tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
#endif   
}


int __stdcall irCmdCallback(char const * capturedCmd) {

   struct translation * cmd;
   static BOOL refreshEnable = TRUE;
   static DWORD repeatDelayTime;
   
   //printf("delay: %s\n", ((long)(repeatDelayTime - GetTickCount()) > 0) ? "TRUE":"FALSE");
   
   // dispose of duplicate callbacks for at least one second after
   // processing a successful command
   if ((long)(repeatDelayTime - GetTickCount()) > 0)
      return 0;
      
   repeatDelayTime = GetTickCount() + 1000;
   
   cmd = lookup(capturedCmd);
   
   // ignore ir noise when TV is off
   //if (!(tvPowered || cmd->id == REMOTE_TV_POWER_TOGGLE))
   //   return;
   
   switch (cmd->id) {
      case REMOTE_MUTE:
         (muteState==MUTE_ON) ? sendCmd(SZ_MUTE_OFF) : sendCmd(SZ_MUTE_ON);
         break;
      case REMOTE_VOLUME_UP:
         sendCmd(SZ_MASTER_VOLUME_UP);
         break;
      case REMOTE_VOLUME_DOWN:
         sendCmd(SZ_MASTER_VOLUME_DOWN);
         break;
      case REMOTE_TV_POWER_TOGGLE:
         tvPowered = ~tvPowered;
         sendCmd(tvPowered ? SZ_REGISTER : SZ_DEREGISTER);
         repeatDelayTime = GetTickCount() + 3000;
         break;
      case REMOTE_TV_VIDEO:
         
         // first press that brings up the video source menu 
         // is used to toggle the surround sound on and off
         
         if (audioSource!=displayState) {
            // reset defaults when switching from an unknown source 
            setAudioSource(VIDEO_AUX);   
            sendCmd(SZ_FRONT_SPEAKERS_ON);
            sendCmd(SZ_CHANNEL_B_SPEAKERS_OFF);
            sendCmd(szDefaultMasterVolume);
         } else {  
            // switch to an unused source as a pseudo mute while settings change
            setAudioSource(VIDEO_AUX);   
            switch(displayState) {
               case TV:
                  displayState=DVD;
                  break;
               case DVD:
                  displayState=LVR_MM;
                  break;
               case LVR_MM:
                  displayState=TV;
                  break;
               default:
                  printf("error: invalid displayState %s\n", cmdIdToSz(displayState));
                  break;
            }
         }   
         setSurroundSound(displayState==DVD);
         setAudioSource(displayState);     
         repeatDelayTime = GetTickCount() + 5000;
         break;
      case REMOTE_TV_MENU_RIGHT:
         setSurroundSound(ON);
         break;   
      case REMOTE_TV_MENU_LEFT:
         setSurroundSound(OFF);
         break;   
      case REMOTE_TV_PIP:
      case REMOTE_VCR_MENU:
         refreshEnable=TRUE;
         setNextForegroundWindow();
         break;
      case REMOTE_TV_MENU_DOWN:
      case REMOTE_TV_MENU_UP:
      case REMOTE_VCR_MENU_DOWN:
      case REMOTE_VCR_MENU_UP:
         sendVirtualKeyCode(cmd->key);
         if (refreshEnable) {
            printf("refresh command issued");
            Sleep(1000);
            sendVirtualKeyCode(REFRESH_EXPLORER);
            refreshEnable=FALSE;
            repeatDelayTime = GetTickCount() + 1000;
         }   
         break;
      case REMOTE_TV_PLAY:
      case REMOTE_VCR_PLAY:
         refreshEnable=TRUE;
      case REMOTE_TV_JUMP_BACKWARD:
      case REMOTE_TV_JUMP_FORWARD:
      case REMOTE_TV_PAUSE:
      case REMOTE_TV_STOP:    
      case REMOTE_VCR_JUMP_FORWARD:
      case REMOTE_VCR_JUMP_BACKWARD:
      case REMOTE_VCR_MENU_LEFT:
      case REMOTE_VCR_MENU_RIGHT:
      case REMOTE_VCR_PAUSE:
      case REMOTE_VCR_STOP:
         sendVirtualKeyCode(cmd->key);
         break;
      default:
         repeatDelayTime = GetTickCount();
         break;
   }
   
   fflush(stdout);
   return 0;
} 


void setSurroundSound(int state) {
   switch(state) {
      case ON:   
         sendCmd(SZ_REAR_SPEAKERS_ON);
         break;
      case OFF:
         sendCmd(SZ_REAR_SPEAKERS_OFF);
         break;
      default:   
         printf("setSurroundSound: unknown state %d\n", state);   
         break;
   }
}   


void setAudioSource(int id) {
   switch(id) {
      case TV:
      case DVD:
      case LVR_MM:
      case VIDEO_AUX:
         sendCmd(cmdIdToSz(id));   
         break;
      default:
         printf("setAudioSource: unknown source %s\n", cmdIdToSz(id));   
         break;
   }      
}


// ignore commands unrelated to mute and audio source state 
int filterCmd(int id) {
   switch(id) {
      case MUTE_ON:
      case MUTE_OFF:
      case GEORGIA:
      case JIMSON:
      case VIDEO_AUX:
      case LVR_MM:
      case DEN_MM:
      case FM:
      case TV:
      case DVD:
      case FIRST_REGISTRATION:
         return id;
      default:
         return UNDEFINED;
   }      
}            


void sendVirtualKeyCode(unsigned vkPress)
{
   KEYBDINPUT kbd[8];
   INPUT      input[8];
   unsigned   i, vkRelease=0;
   
   memset((char *)&kbd, 0, sizeof(KEYBDINPUT[8]));
   memset((char *)&input, 0, sizeof(INPUT[8]));
   
   for (i=0; i<4 && (vkPress & 0xFF); i++) {
      kbd[i].wVk = vkPress & 0xFF;
      input[i].type = INPUT_KEYBOARD;
      input[i].ki = kbd[i];
      vkRelease |= vkPress & 0xFF;
      vkPress >>= 8;
      if (vkPress & 0xFF)
         vkRelease <<= 8;
   }
   
   for (; i<7 && (vkRelease & 0xFF); i++) {
      kbd[i].wVk = vkRelease & 0xFF;
      kbd[i].dwFlags |= KEYEVENTF_KEYUP;
      input[i].type = INPUT_KEYBOARD;
      input[i].ki = kbd[i];
      vkRelease >>= 8;
   }    
   
   SendInput(i, input, sizeof(INPUT));
}


char * processArgs(int argc, char ** argv) {
   int i;
   char * szPort = "0";

   for (i=0;i<argc;i++) {
      if(!strcmp(argv[i],"-port"));
      else if (i && !strcmp(argv[i-1],"-port"))
         szPort = argv[i];
      else if (i) {
         printf(usage);
         exit(0);
      }   
   }
   
   return szPort;
}   


// check if the application filename ends with "Console.exe"
char isConsoleMode() {
    STARTUPINFO si;
    static char consoleMode = UNDEFINED;
    if (consoleMode==UNDEFINED) {
       char * consoleSuffix = "Console.exe";
       GetStartupInfo(&si);
       consoleMode = !(strcmp(&si.lpTitle[strlen(si.lpTitle)-strlen(consoleSuffix)],
                              consoleSuffix));
    }                          
    return consoleMode;                
}      


char * getHostname() {
    char * hostname;
    static char szHostname[128];
    if (!(hostname=getenv("COMPUTERNAME")))
        errMsgExit("getHostname: COMPUTERNAME environment variable not found");
    strcpy(szHostname, hostname);
    strlwr(szHostname);    
    return szHostname;
}


void errMsgExit(char * msg) {
    MessageBox(
        NULL,
        msg,
        "Launch Video",
        MB_ICONERROR | MB_OK
    );
    exit(1);
}


void dispatchThread()
{
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


// setNextForegroundWindow uses GetNextWindow and GetWindowText to 
// maintain a list of windows that partially match an entry in the 
// dirList array.  After updating the list the next round-robin
// entry in the list is used to call SetForegroundWindow.  If no windows
// with partially matching titles exist, SetForegroundWindow is not called.
//
// Although dirList can contain any arbitrary window title, the intended
// use is to iterate over explorer windows.  In order to differentiate
// explorer windows, it is important for the title to fully qualify the
// current path.  Go to Organize/Folder Options/View and check
// "Display the full path in the title bar (Classic theme only)"

void setNextForegroundWindow() {
   unsigned i, j, titleLen;
   char title[1024];
   char titleFG[1024];
   char * titleNext;
   HWND hWndNext = NULL;
   HWND hWnd = GetTopWindow(NULL);
   HWND hWndFG = GetForegroundWindow();
   GetWindowText(hWndFG, titleFG, sizeof(titleFG));
   
   for (i=0; i<sizeof(windowList)/sizeof(struct windowList); i++)
      windowList[i].count=0;
   
   // iterate through every desktop window.  If a dirList entry
   // partially matches a window's title, save the window's handle
   // to a windowList struct and increment the count 
   while(hWnd) {
      titleLen = GetWindowText(hWnd, title, sizeof(title));
      for (i=0; titleLen>0 && i<sizeof(dirList)/sizeof(char *); i++) {
         if (strstr(title,dirList[i])) {
            // this window's title matches or is a subdirectory of dirList[i]
            for (j=0; j<sizeof(windowList)/sizeof(struct windowList); j++) {
               if (strstr(windowList[j].title, title) ||
                   !windowList[j].title[0]) {
                  // either window title already exists in windowList
                  // OR we've reached the end of the list   
                  windowList[j].count++;
                  windowList[j].hWnd = hWnd;
                  if (!windowList[j].title[0])
                     strcpy(windowList[j].title, title);
                  break;
               }
            }
         }
      }            
      hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
   }
   
   // now iterate through windowList to find the next matching window handle
   for (i=0; i<sizeof(windowList)/sizeof(struct windowList) &&
             hWndNext==NULL; i++) {
      if (strstr(windowList[i].title, titleFG)) {
         for (j=i+1; j<sizeof(windowList)/sizeof(struct windowList); j++) {
            if (windowList[j].count) {
               hWndNext = windowList[j].hWnd;
               titleNext = windowList[j].title;
               break;
            }   
         }
         if (hWndNext==NULL) {
            // wrap to the beginning of windowList to continue the search
            for (j=0; 
                 j<i && j<sizeof(windowList)/sizeof(struct windowList); 
                 j++) {
               if (windowList[j].count) {
                  hWndNext = windowList[j].hWnd;
                  titleNext = windowList[j].title;
                  break;
               }   
            }
         }   
      }
   }

   if (hWndNext==NULL && 
       windowList[0].count &&
       hWndFG != windowList[0].hWnd) {
      // set hWndNext to the first matching window 
      hWndNext = windowList[0].hWnd;
      titleNext = windowList[0].title;
   }
         
   if (hWndNext) {
      ShowWindow(hWndNext, SW_SHOWNORMAL);
      SetForegroundWindow(hWndNext);
   } 
   
   printf("setNextForegroundWindow: %s\n", hWndNext ? titleNext : "not found");
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


LRESULT WINAPI exitHandler(HWND hwnd, 
                           UINT msg, 
                           WPARAM wParam, 
                           LPARAM lParam) 
{
    switch(msg) {
       case WM_ENDSESSION:
       case WM_CLOSE:
          WSACleanup();      // release winsock
		  PostQuitMessage(0);
          break;
       default:   
        return DefWindowProc(hwnd, msg, wParam, lParam); 
     }   
	return 0; 
}                               


void help() {
   printf( "type \'q\' to quit\n\n");
}



