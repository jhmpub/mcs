#include <ctype.h>
#include <winsock2.h>
#include <winuser.h>
#include <stdio.h>
#include <unistd.h>
#include "tira.h"
#include "xantech_codes.h"
#include "yamaha_codes.h"
#include "common.h"
#include "socket.h"
#include "common.h"
#include "xantech_codes.c"
#include "yamaha_codes.c"
#include "hdmi_switch_codes.c"

#ifdef CONSOLE_MODE
#include <termios.h>
static struct termios old_tio, new_tio;
#endif

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

int debug=FALSE;
int initializing=FALSE;
int audioSource=VIDEO_AUX;
int station=FM1;
int currentEffectSpeaker=0;
int audioPowerState, muteState=MUTE_OFF;
int registrationCount=0, surroundProfile=SURROUND_STD;
int channel_a_director=CHANNEL_A_TO_FRONT_SPEAKERS;
int rear_speaker_selector=REAR_SPEAKERS_FROM_CHANNEL_A;
int channel_a_relay=CHANNEL_A_OFF;
int bSpeakerState=CHANNEL_B_OFF;
int subwoofer_relay=SUBWOOFER_OFF;
int center_speaker_relay=CENTER_SPEAKER_OFF;
int subwooferState, LFE_State;
int rearSpeakerState=REAR_SPEAKERS_OFF;
int frontSpeakerState=FRONT_SPEAKERS_OFF; 
int hdmiSwitchState=UNDEFINED;
int ttyExit=FALSE;
int undefined=UNDEFINED;

#define VK_Q  0x51
#define VK_CR 0x0D
#define SC_Q  0x10
#define SC_CR 0x1C

INPUT_RECORD inputQ[3] = {
   KEY_EVENT,
   {TRUE, 1, VK_CR, SC_CR, '\r', 0},
   KEY_EVENT,
   {TRUE, 1, VK_Q, SC_Q, 'q', 0},
   KEY_EVENT,
   {TRUE, 1, VK_CR, SC_CR, '\r', 0},
};   

HMODULE handle = 0;

struct serverState {
   int rearSpeakerState; 
   int frontSpeakerState; 
   int bSpeakerState;
   int audioSource;
} serverState, *preserved=NULL;


struct tiraCmd tiraCmd[] = { 

   // yamaha receiver commands
   {UNDEFINED,
    SZ_UNDEFINED,
    &undefined,
    0,
    NULL},
    
   {AUDIO_POWER_ON,
    SZ_AUDIO_POWER_ON,
    &audioPowerState,
    sizeof(irPowerOn),
    irPowerOn},
    
   {AUDIO_POWER_OFF,
    SZ_AUDIO_POWER_OFF,
    &audioPowerState,
    sizeof(irPowerOff),
    irPowerOff},
    
   {GEORGIA,
    SZ_GEORGIA,
    &audioSource,
    sizeof(irCD),
    irCD},
    
   {JIMSON,
    SZ_JIMSON,
    &audioSource,
    sizeof(irCblSat),
    irCblSat},
    
   {LVR_MM,
    SZ_LVR_MM,
    &audioSource,
    sizeof(irCDR),
    irCDR},
    
   {DEN_MM,
    SZ_DEN_MM,
    &audioSource,
    sizeof(irVideoAux),
    irVideoAux},
    
   {FM,
    SZ_FM,
    &audioSource,
    sizeof(irFm),
    irFm},
    
   {TV,
    SZ_TV,
    &audioSource,
    sizeof(irTv),
    irTv},
    
   {DVD,
    SZ_DVD,
    &audioSource,
    sizeof(irDvd),
    irDvd},
    
   {DVR,
    SZ_DVR,
    &audioSource,
    sizeof(irCDR),
    irCDR},
    
   {VIDEO_AUX,
    SZ_VIDEO_AUX,
    &audioSource,
    sizeof(irVideoAux),
    irVideoAux},
    
   {FM1,
    SZ_FM1,
    &station,
    sizeof(irFm1),
    irFm1},
    
   {FM2,
    SZ_FM2,
    &station,
    sizeof(irFm2),
    irFm2},
    
   {FM3,
    SZ_FM3,
    &station,
    sizeof(irFm3),
    irFm3},
    
   {FM4,
    SZ_FM4,
    &station,
    sizeof(irFm4),
    irFm4},
    
   {FM5,
    SZ_FM5,
    &station,
    sizeof(irFm5),
    irFm5},
    
   {FM6,
    SZ_FM6,
    &station,
    sizeof(irFm6),
    irFm6},
    
   {FM7,
    SZ_FM7,
    &station,
    sizeof(irFm7),
    irFm7},
    
   {FM8,
    SZ_FM8,
    &station,
    sizeof(irFm8),
    irFm8},
    
   {MUTE_ON,
    SZ_MUTE_ON,
    &muteState,
    sizeof(irMuteToggle),
    irMuteToggle},
    
   {MUTE_OFF,
    SZ_MUTE_OFF,
    &muteState,
    sizeof(irMuteToggle),
    irMuteToggle},
    
   {MUTE_TOGGLE,
    SZ_MUTE_TOGGLE,
    &muteState,
    sizeof(irMuteToggle),
    irMuteToggle},
    
   {FIRST_REGISTRATION,
    SZ_FIRST_REGISTRATION,
    &undefined,
    0,
    NULL},
    
   {REGISTER,
    SZ_REGISTER,
    &undefined,
    0,
    NULL},
    
   {DEREGISTER,
    SZ_DEREGISTER,
    &undefined,
    0,
    NULL},

   {REREGISTER,
    SZ_REREGISTER,
    &undefined,
    0,
    NULL},
    
   {RESTORE_STATE,
    SZ_RESTORE_STATE,
    &undefined,
    0,
    NULL},
    
   {MASTER_VOLUME,
    SZ_MASTER_VOLUME,
    &masterVolume,
    0,
    NULL},
    
   {MASTER_VOLUME_UP,
    SZ_MASTER_VOLUME_UP,
    &masterVolume,
    sizeof(irMasterVolumeUp),
    irMasterVolumeUp},
    
   {MASTER_VOLUME_DOWN,
    SZ_MASTER_VOLUME_DOWN,
    &masterVolume,
    sizeof(irMasterVolumeDn),
    irMasterVolumeDn},
    
   {CENTER_VOLUME,
    SZ_CENTER_VOLUME,
    &centerVolume,
    0,
    NULL},
    
   {CENTER_VOLUME_UP,
    SZ_CENTER_VOLUME_UP,
    &centerVolume,
    sizeof(irMenuRight),
    irMenuRight},
    
   {CENTER_VOLUME_DOWN,
    SZ_CENTER_VOLUME_DOWN,
    &centerVolume,
    sizeof(irMenuLeft),
    irMenuLeft},
    
   {SURROUND_VOLUME,
    SZ_SURROUND_VOLUME,
    &surroundRightVolume,
    0,
    NULL},
    
   {SURROUND_RIGHT_VOLUME,
    SZ_SURROUND_RIGHT_VOLUME,
    &surroundRightVolume,
    0,
    NULL},
    
   {SURROUND_RIGHT_VOLUME_UP,
    SZ_SURROUND_RIGHT_VOLUME_UP,
    &surroundRightVolume,
    sizeof(irMenuRight),
    irMenuRight},
    
   {SURROUND_RIGHT_VOLUME_DOWN,
    SZ_SURROUND_RIGHT_VOLUME_DOWN,
    &surroundRightVolume,
    sizeof(irMenuLeft),
    irMenuLeft},
    
   {SURROUND_LEFT_VOLUME,
    SZ_SURROUND_LEFT_VOLUME,
    &surroundLeftVolume,
    0,
    NULL},
    
   {SURROUND_LEFT_VOLUME_UP,
    SZ_SURROUND_LEFT_VOLUME_UP,
    &surroundLeftVolume,
    sizeof(irMenuRight),
    irMenuRight},
    
   {SURROUND_LEFT_VOLUME_DOWN,
    SZ_SURROUND_LEFT_VOLUME_DOWN,
    &surroundLeftVolume,
    sizeof(irMenuLeft),
    irMenuLeft},
    
   {SUBWOOFER_VOLUME,
    SZ_SUBWOOFER_VOLUME,
    &subwooferVolume,
    0,
    NULL},
    
   {SUBWOOFER_VOLUME_UP,
    SZ_SUBWOOFER_VOLUME_UP,
    &subwooferVolume,
    sizeof(irMenuRight),
    irMenuRight},
    
   {SUBWOOFER_VOLUME_DOWN,
    SZ_SUBWOOFER_VOLUME_DOWN,
    &subwooferVolume,
    sizeof(irMenuLeft),
    irMenuLeft},
    
   {SURROUND_MIN,
    SZ_SURROUND_MIN,
    &surroundProfile,
    0,
    NULL},
    
   {SURROUND_STD,
    SZ_SURROUND_STD,
    &surroundProfile,
    0,
    NULL},
    
   {SURROUND_MAX,
    SZ_SURROUND_MAX,
    &surroundProfile,
    0,
    NULL},
    
   {MENU_EFFECT_SPEAKER_LEVEL,
    SZ_MENU_EFFECT_SPEAKER_LEVEL,
    &undefined,
    sizeof(irMenuEffectSpeakerLevel),
    irMenuEffectSpeakerLevel},
    
   {MENU_SETTINGS,
    SZ_MENU_SETTINGS,
    &undefined,
    sizeof(irMenuSet),
    irMenuSet},
    
   {MENU_UP,
    SZ_MENU_UP,
    &undefined,
    sizeof(irMenuUp),
    irMenuUp},
    
   {MENU_DOWN,
    SZ_MENU_DOWN,
    &undefined,
    sizeof(irMenuDown),
    irMenuDown},
    
   {MENU_LEFT,
    SZ_MENU_LEFT,
    &undefined,
    sizeof(irMenuLeft),
    irMenuLeft},
    
   {MENU_RIGHT,
    SZ_MENU_RIGHT,
    &undefined,
    sizeof(irMenuRight),
    irMenuRight},
    
   {DSP_EFFECT_ON,
    SZ_DSP_EFFECT_ON,
    &undefined,
    sizeof(irDspEffectToggle),
    irDspEffectToggle},
    
   {DSP_EFFECT_OFF,
    SZ_DSP_EFFECT_OFF,
    &undefined,
    sizeof(irDspEffectToggle),
    irDspEffectToggle},
    
   {DSP_EFFECT_TOGGLE,
    SZ_DSP_EFFECT_TOGGLE,
    &undefined,
    sizeof(irDspEffectToggle),
    irDspEffectToggle},
    
   {DSP_EFFECT_TOGGLE_ENABLE,
    SZ_DSP_EFFECT_TOGGLE_ENABLE,
    &undefined,
    0,
    NULL},
    
   {DSP_EFFECT_TOGGLE_DISABLE,
    SZ_DSP_EFFECT_TOGGLE_DISABLE,
    &undefined,
    0,
    NULL},
    
   {DSP_DOLBY_NORMAL,
    SZ_DSP_DOLBY_NORMAL,
    &undefined,
    sizeof(irDspDolbyNormal),
    irDspDolbyNormal},
    
    // xantech control commands
   {CHANNEL_A_TO_FRONT_SPEAKERS,
    SZ_CHANNEL_A_TO_FRONT_SPEAKERS,
    &channel_a_director,
    sizeof(irOff0),
    irOff0},
    
   {CHANNEL_A_TO_REAR_SPEAKERS,
    SZ_CHANNEL_A_TO_REAR_SPEAKERS,
    &channel_a_director,
    sizeof(irOn0),
    irOn0},
    
   {REAR_SPEAKERS_FROM_CHANNEL_A,
    SZ_REAR_SPEAKERS_FROM_CHANNEL_A,
    &rear_speaker_selector,
    sizeof(irOff1),
    irOff1},
    
   {REAR_SPEAKERS_FROM_REAR_CHANNEL,
    SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL,
    &rear_speaker_selector,
    sizeof(irOn1),
    irOn1},
    
   {CHANNEL_A_OFF,
    SZ_CHANNEL_A_OFF,
    &channel_a_relay,
    sizeof(irOff2),
    irOff2},
    
   {CHANNEL_A_ON,
    SZ_CHANNEL_A_ON,
    &channel_a_relay,
    sizeof(irOn2),
    irOn2},
    
   {CENTER_SPEAKER_OFF,
    SZ_CENTER_SPEAKER_OFF,
    &center_speaker_relay,
    sizeof(irOff3),
    irOff3},
    
   {CENTER_SPEAKER_ON,
    SZ_CENTER_SPEAKER_ON,
    &center_speaker_relay,
    sizeof(irOn3),
    irOn3},
    
   {CHANNEL_B_OFF,
    SZ_CHANNEL_B_OFF,
    &bSpeakerState,
    sizeof(irOff4),
    irOff4},
    
   {CHANNEL_B_ON,
    SZ_CHANNEL_B_ON,
    &bSpeakerState,
    sizeof(irOn4),
    irOn4},
    
   {SUBWOOFER_OFF,
    SZ_SUBWOOFER_OFF,
    &subwoofer_relay,
    sizeof(irOff5),
    irOff5},
    
   {SUBWOOFER_ON,
    SZ_SUBWOOFER_ON,
    &subwoofer_relay,
    sizeof(irOn5),
    irOn5},
    
   {B_SPEAKERS_ON,
    SZ_B_SPEAKERS_ON,
    &bSpeakerState,
    sizeof(irOn4),
    irOn4},
    
   {B_SPEAKERS_OFF,
    SZ_B_SPEAKERS_OFF,
    &bSpeakerState,
    sizeof(irOff4),
    irOff4},
    
   {REAR_SPEAKERS_ON,
    SZ_REAR_SPEAKERS_ON,
    &rearSpeakerState,
    0,
    NULL},
    
   {REAR_SPEAKERS_OFF,
    SZ_REAR_SPEAKERS_OFF,
    &rearSpeakerState,
    0,
    NULL},
    
   {FRONT_SPEAKERS_ON,
    SZ_FRONT_SPEAKERS_ON,
    &frontSpeakerState,
    0,
    NULL},
    
   {FRONT_SPEAKERS_OFF,
    SZ_FRONT_SPEAKERS_OFF,
    &frontSpeakerState,
    0,
    NULL},
    
   // hdmi switch commands
   {HDMI_SET_TOP_BOX_SOURCE,
    SZ_HDMI_SET_TOP_BOX_SOURCE,
    &hdmiSwitchState,
    sizeof(hdmiSourceInput1),
    hdmiSourceInput1},
   
   {HDMI_DVD_SOURCE,
    SZ_HDMI_DVD_SOURCE,
    &hdmiSwitchState,
    sizeof(hdmiSourceInput2),
    hdmiSourceInput2},
    
   {HDMI_DVR_SOURCE,
    SZ_HDMI_DVR_SOURCE,
    &hdmiSwitchState,
    sizeof(hdmiSourceInput3),
    hdmiSourceInput3},
   
   // general commands 
   {EXIT,
    SZ_EXIT,
    &undefined,
    0,
    NULL}
};    


void printWinsockError(char * errorMsg) {
   printf("\n%s: winsock error code %d\n", errorMsg, WSAGetLastError()); 
}


void updateState(const char * szMsg) {
   setState(szMsg);
   broadcastMsg(szMsg);
}

     
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

   char * vol, * level=NULL;

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
   char msgBuffer[strlen(msg)+1], * vol;
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


void processClientMsg(struct socketDescriptor * sd, char * msg) {
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
   
   processClientCmd(msg);
}

void processClientDisconnect(struct socketDescriptor * sd) { 
   if (sd->registered && *sd->socket) {
       // registered and connection still alive
       processClientDisconnect(); 
   }    
   sd->registered = FALSE;   
}


void createProcess(const char * exeFile, const char * arg) {
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   memset(&si,0,sizeof(si));
   si.cb = sizeof(si);
   memset(&pi,0,sizeof(pi));
   
   char moduleDir[strlen(exeFile)+1], *slash;
   strcpy(moduleDir,exeFile);
   if(slash=strrchr(moduleDir,'\\'))
      *slash=0;
      
   char cmdLine[strlen(exeFile)+strlen(arg)+2];
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
} 


char * szToLower(char * sz) {
   char *p=sz; 
   for (; *p; ++p) *p=tolower(*p);
   return sz;
}


// atomic compare and swap 
// if lock == FALSE, lock = TRUE; return FALSE
// if lock == TRUE, lock = TRUE; return TRUE
LONG isLocked(volatile LONG * lock) {
   return InterlockedCompareExchange(lock, TRUE, FALSE);
}

void unlock(volatile LONG * lock) { *lock=FALSE; }


// findWindow is a WinAPI FindWindow replacement that finds the first
// window with a case insenstive version of the given string and then 
// returns the corresponding HWND or NULL 
HWND findWindow(const char * sz) {
   int i, titleLen = 0;
   char windowTxt[256];
   char title[strlen(sz) + 1];
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


int ttyLoop() {
   char c;
   
   help();
   stty_unbuffered();
   ttyExit=TRUE;
   
   while (1) {
      printf(">");
      c=getchar();
      
      switch (c) {
         case '\n':
            break;
         case 'q':
         case 'Q':
            if (ttyExit) {
               printf("\n");
               ttyExit=FALSE;
               processClientCmd(SZ_EXIT);
            } 
            return 0;
         default:
            help();
      }
   }
   return 0; 
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
   // restore to flushing buffer after a CR
   tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
#endif   
}

BOOL isConsoleMode() {
#ifdef CONSOLE_MODE
   return TRUE;
#else   
   return FALSE;
#endif
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
          processClientCmd(SZ_EXIT);
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
       processClientCmd(SZ_EXIT);
       return TRUE;
 
    // Pass other signals to the next handler. 
    case CTRL_BREAK_EVENT: 
    case CTRL_LOGOFF_EVENT: 
       return FALSE; 
 
    default: 
       return FALSE; 
  } 
} 


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


void loadTiraDll() {

   HMODULE handle;
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
   
   if ( !(handle=LoadLibrary(dll)) )
      printErrorAndExit("LoadLibrary(Tira2.dll)");
      
   if ( !(tira_init = (tira_init_t) GetProcAddress(handle, "tira_init")) )
       printErrorAndExit("GetProcAddress(tira_init)");
       
   if ( !(tira_cleanup = (tira_cleanup_t) GetProcAddress(handle, "tira_cleanup")) )
       printErrorAndExit("GetProcAddress(tira_cleanup)");
       
   if ( !(tira_set_handler = (tira_set_handler_t) GetProcAddress(handle, "tira_set_handler")) )
       printErrorAndExit("GetProcAddress(tira_handler)");
       
   if ( !(tira_start = (tira_start_t) GetProcAddress(handle, "tira_start")) )
       printErrorAndExit("GetProcAddress(tira_start)");
       
   if ( !(tira_stop = (tira_stop_t) GetProcAddress(handle, "tira_stop")) )
       printErrorAndExit("GetProcAddress(tira_stop)");
       
   if ( !(tira_start_capture = (tira_start_capture_t) GetProcAddress(handle, "tira_start_capture")) )
       printErrorAndExit("GetProcAddress(tira_start_capture)");
       
   if ( !(tira_cancel_capture = (tira_cancel_capture_t) GetProcAddress(handle, "tira_cancel_capture")) )
       printErrorAndExit("GetProcAddress(tira_cancel_capture)");
       
   if ( !(tira_get_captured_data = (tira_get_captured_data_t) GetProcAddress(handle, "tira_get_captured_data")) )
       printErrorAndExit("GetProcAddress(tira_get_captured_data)");
       
   if ( !(tira_transmit = (tira_transmit_t) GetProcAddress(handle, "tira_transmit")) )
       printErrorAndExit("GetProcAddress(tira_transmit)");
       
   if ( !(tira_delete = (tira_delete_t) GetProcAddress(handle, "tira_delete")) )
       printErrorAndExit("GetProcAddress(tira_delete)");
}


void unloadTiraDll() {
   tira_cleanup();
   FreeLibrary(handle);
}


int processArgs(int argc, char ** argv) {
   int i, startupDelay, comPort=0;

   for (i=0;i<argc;i++) {
   
      if (!strcmp(argv[i],"-d"))
         debug=TRUE;
      else if(!strcmp(argv[i],"-port") || !strcmp(argv[i],"-delay"));
      else if (i && !strcmp(argv[i-1],"-port"))
         comPort = atoi(argv[i]);
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
         usage();
         exit(0);
      }   
   }
   return comPort;
}   

void usage() { 
   printf("\nUsage: %s [-d] [-delay <seconds>] [-port <port>]\n", moduleName);
}


#ifdef NO_MCS
const char * moduleName = "???";
void processClientCmd(const char * szCmd) {}
void processClientDisconnect() {}
const char * tira_dll_absolute_path = 
   "\\\\georgia\\backup\\install\\drivers\\interface\\usb\\tira\\sdk\\Tira2.dll";
#endif

