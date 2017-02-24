// This program serves as a remote control agent (rcAgent) that runs as a
// backround process for universal ir and smart remotes.  If a translation
// is found for a command, this process will either forward the command
// as an IP datagram to the audio receiver agent or synthesize pressing
// a shortcut key to the window that has focus.  This process also maintains
// TV and DVD state information for remote clients.
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
// The rcAgent application is intended to be run from the startup folder.
// The rcAgentConsole application is intended to be used for debugging.
//
// Build Information
// Build the targets with the build.bat script in the directory with this file.
// The rcAgent target is built to run as a detached windows background
// process during startup.  The rcAgentConsole target is used for debugging.
//
// 2008 Sep 10 jhm original creation
// 2009 Apr 10 jhm added IR cmd translations for videolan 
//                 media player
// 2013 Apr 28 jhm added IR cmd translations for windows media center
// 2015 Dec 24 jhm added a thread to listen for audio control notifications
// 2016 Jul 17 jhm added smart remote functionality

// SendInput() is only declared for Windows XP and later
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winsock2.h>
#include <psapi.h>
#include <stdio.h>
#include "../../common/tira.h"
#include "../../common/xantech_codes.h"
#include "../../common/yamaha_codes.h"
#include "../../common/common.h"
#include "../../common/extern.h"
#include "../../common/socket.h"
#include "rcAgent.h"

extern LinkedList * notifyList;

int main(int argc, char ** argv) {

   int error, comPort;
   comPort = processArgs(argc, argv);
   
   loadTiraDll();
   printf("Tira library loaded\n");
   tira_init();
   initTcpServer(initWinsock(), MEDIA_CONTROL_PORT+1);

   if (!debug) {
      int port = comPort ? comPort : DEFAULT_COM_PORT;
      printf("Tira activat%s on com port %d\n", 
            (error=tira_start(port-1)) ? "ion failed" : "ed", port);
      if (error) { usage(); exit(1); }
   
      printf("Callback capture activat%s\n", 
             (error=tira_set_handler(irRxCmdCallback)) ? "ion failed" : "ed");
      if (error) exit(1);
   }   
   
   HANDLE hThread = initExitHandler();
   initConsoleModeExitHandler();
   
   arAgentSd.host = 
      (char *) (debug ? AUDIO_RECEIVER_DEBUG_HOSTNAME : AUDIO_RECEIVER_AGENT_HOSTNAME);
   initClientThread(&arAgentSd);
   initClientThread(&itachSd);
   
   isConsoleMode() ?
      ttyLoop() :
      WaitForSingleObject(hThread,INFINITE);
      
}


// generic routinte to create a thread that maintains a connection 
// for receiving agent status notifications
void initClientThread(struct socketDescriptor * sd) {

   establishConnection(sd);

   if (CreateThread(NULL,  // default security
                    0,     // default stack size
                    (LPTHREAD_START_ROUTINE)&rxClientThread,
                    sd,    // thread parameter
                    0,     // run thread immediately
                    NULL)  // ignore thread id
        == NULL) {
      char description[strlen("init") + strlen(sd->description)+1];
      printErrorAndExit((const char *)strcat(strcpy(description,"init"),sd->description));
   }
} 


// update state information received from the audio receiver agent
// This function confirms completion of a command request and performs
// initialization sequences based on the audio/video system state
void processArAgentNotification(char * msg) {

   printf("processArAgentNotification: %s\n", msg);
   
   int cmdId=tiraCmdSzToId(msg);
   
   if (cmdId!=UNDEFINED)
      updateState(msg);
      
   switch(cmdId) {
      case TV:
      case DVD:
      case LVR_MM:
      case DEN_MM:
         if (displayMode==UNDEFINED && tvPowered) { 
            nextMode=displayMode=getVideoMode(cmdId);
            // initialize the current displayMode according to a registered 
            // client's audio state by setting the TV input 
            //  tv: video+4 (hdmi switch input)
            // dvd: video+2 (dvd component input)
            // dvr: video+4 (hdmi switch input)
            sendItachCmd(MODE_MENU, TV_MODE, INDEPENDENT, 1500);
            sendItachCmd(displayMode, TV_MODE, DEPENDENT, 4000);   
            
            // and the hdmi switch source
            setVideoSource(displayMode);
         }   
         break;      
      case FIRST_REGISTRATION:
         if (displayMode==UNDEFINED) {
            sendArCmd(SZ_FRONT_SPEAKERS_ON);
            sendArCmd(szDefaultMasterVolume);
            setVideoMode(TV);    // force initialization to a known state
            setAudioMode(TV);
         }   
         else {   
            setSurroundSound(displayMode==DVD);
            sendArCmd(SZ_FRONT_SPEAKERS_ON);
            sendArCmd(szDefaultMasterVolume);
            setAudioSource(displayMode);
         }   
         break;
      case RESTORE_STATE:
         restoreReceiverState();   
         break;
      default:
         break;
   }
   
}

// handle arAgent and itach notifications
void processAgentNotification(const char * agent, char * msg) {
   if (!strcmp(agent, arAgentSd.host))
      processArAgentNotification(msg);
   else {
      printf("processAgentNotification from %s: %s\n", agent, msg);
      if (strstr(msg, "completeir"))
         itachCmd.irComplete=TRUE;
   }      
}

void processClientDisconnect() {}
void processServerConnect(const char * agent) {
   if (!strcmp(agent, arAgentSd.host) && tvPowered) {
         sendArCmd(SZ_REREGISTER);
   }
}
      
void sendArCmd(const char * szCmd) {
   if (arAgentSocket) 
      sendMsg(&arAgentSd, szCmd);
}

// the default sendItachCmd has parameters where subsequent sendItachCmds are
// INDEPENDENT and zero processing command delay
void sendItachCmd(int id, int mode) { sendItachCmd(id, mode, INDEPENDENT, 0); }

// queue up a sendItachCmdThread
void sendItachCmd(int id, int mode, int dependent, int delay) {
   static int sequence=0;
   struct rcCmd * cmd = cmdIdToRcCmd(id);
  
   if (!cmd->irTx || !(mode==TV_MODE || mode==DVD_MODE))
      return;
      
   struct irTx * irTx = &cmd->irTx[mode];
  
   if (!dependent)
      itachCmd.prerequisiteFailure=FALSE;
   
   if (debug)
      printf("sendItachCmd: %s\n", irTx->sz);
   else if (itachSocket && irTx->code) {
      sendItachCmdThreadStart(new ItachCmdParam(irTx, dependent, sequence++, delay));
   }      
}


void restoreReceiverState() {
  sendArCmd(tiraCmdIdToSz(AUDIO_POWER_ON));
  sendArCmd(SZ_VIDEO_AUX);
  sendArCmd(tiraCmdIdToSz(rearSpeakerState));
  sendArCmd(tiraCmdIdToSz(frontSpeakerState));
  sendArCmd(tiraCmdIdToSz(bSpeakerState));
  sendArCmd(tiraCmdIdToSz(MASTER_VOLUME));
  sendArCmd(tiraCmdIdToSz(CENTER_VOLUME));
  sendArCmd(tiraCmdIdToSz(SURROUND_VOLUME));
  sendArCmd(tiraCmdIdToSz(SUBWOOFER_VOLUME));
  sendArCmd(tiraCmdIdToSz(surroundProfile));
  sendArCmd(tiraCmdIdToSz(station));
  sendArCmd(tiraCmdIdToSz(audioSource));
  sendArCmd(tiraCmdIdToSz(audioPowerState));
  sendArCmd(tiraCmdIdToSz(muteState));
}


void sendStateInformation() {
  sendMsg(tiraCmdIdToSz(audioPowerState));
  sendMsg(SZ_VIDEO_AUX);
  sendMsg(tiraCmdIdToSz(rearSpeakerState));
  sendMsg(tiraCmdIdToSz(frontSpeakerState));
  sendMsg(tiraCmdIdToSz(bSpeakerState));
  sendMsg(tiraCmdIdToSz(MASTER_VOLUME));
  sendMsg(tiraCmdIdToSz(CENTER_VOLUME));
  sendMsg(tiraCmdIdToSz(SURROUND_VOLUME));
  sendMsg(tiraCmdIdToSz(SUBWOOFER_VOLUME));
  sendMsg(tiraCmdIdToSz(surroundProfile));
  sendMsg(tiraCmdIdToSz(station));
  sendMsg(tiraCmdIdToSz(audioSource));
  sendMsg(tiraCmdIdToSz(muteState));
}   
   

// return TRUE if given irRx has capturedCode in it's array of possible codes
int irRxHasCode(struct irRx * irRx, char const * capturedCode) {

   int j,k;
   const char * tableCode;
   
   for(j=0; irRx && j<irRx->size; j++) {
      tableCode = irRx->code[j];
      
      for (k=0;
           k<CMD_CODE_LENGTH &&
           capturedCode[k]==tableCode[k]; k++);
           
      if (k==CMD_CODE_LENGTH)
         return TRUE;
   }    
   
   return FALSE;
}                


// return the remote control command structure associated with the
// given tira ir code string
struct rcCmd * irRxCmdToRcCmd(char const * capturedCmd) {

   int i,j,k;
   const char * tableCode;
   
   for (i=0; i<sizeof(rcCmd)/sizeof(struct rcCmd); i++) {
      for(j=0; rcCmd[i].irRx && j<rcCmd[i].irRx->size; j++) {
         tableCode = rcCmd[i].irRx->code[j];
         
         for (k=0;
              k<CMD_CODE_LENGTH &&
              capturedCmd[k]==tableCode[k]; k++);
              
         if (k==CMD_CODE_LENGTH)
            return &rcCmd[i];
      }    
   }
   
   return &rcCmd[UNDEFINED];

}                


// tira callback for ir remote clients (e.g. itach, universal remote, etc.)
int __stdcall irRxCmdCallback(char const * capturedCmd) {

   static unsigned sequence=0;
   DWORD discardPeriod=0;
   int duplicate=FALSE;
   char irRxDebug[1024];
      
   struct rcCmd * cmd = irRxCmdToRcCmd(capturedCmd);
   irRxTime=GetTickCount();
   
   if (cmd->id == UNDEFINED) {      
      snprintf(irRxDebug, 
         sizeof(irRxDebug),
         "Notice: capturedCmd msg match not found for %s\n", 
         capturedCmd); 
   }
   else {
      discardPeriod = cmd->id==VIDEO_POWER ? 5000 : 500;
      duplicate = GetTickCount() - cmd->irRx->timestamp < discardPeriod;
      snprintf(irRxDebug,  
         sizeof(irRxDebug),
         "%s %s: %s->%s\n",
         duplicate ? "irRx discarding duplicate" : "irRx match found",
         szTime(),
         capturedCmd, 
         cmd->irRx->sz);
      // timestamp receipt to throw away repeated commands generated when a user
      // presses and holds down a universal remote button 
      cmd->irRx->timestamp=GetTickCount();
   } 
      
   if (irRxHasCode(itachCmd.irRxPending, capturedCmd)) {
      strncpy(itachCmd.debug, irRxDebug, sizeof(itachCmd.debug)); 
      itachCmd.irRxPending = NULL;
      return 0;
   }   
   
   if (cmd->id == IR_IGNORE ||
       (duplicate || cmd->id == UNDEFINED) && !isConsoleMode() ) {
       // no need to report IR_IGNORE (itach IR commands) OR
       // report duplicate OR UNDEFINED commands unless in console mode  
      return 0;
   }   
   
   // start a thread to process tira cmds sequentially
   tiraCmdThreadStart(new TiraCmd(cmd->sz, irRxDebug, duplicate, sequence++));
   
   return 0;
}


void tiraCmdThreadStart(TiraCmd * param) {
   HANDLE hThread;
   if (hThread=CreateThread(
         NULL,    // default security
         0,       // default stack size
         (LPTHREAD_START_ROUTINE) tiraCmdThread,
         param,   // thread parameter
         0,       // run thread immediately
         NULL)    // ignore thread id
      ) {
      CloseHandle(hThread);
   } else {
      printf("tiraCmdThreadStart failed\n");
   }   
}


DWORD tiraCmdThread(LPVOID param) {
   TiraCmd * tiraCmd = (TiraCmd *) param;
   
   while (tiraCmd->sequence != tiraCmdActive ||
          isLocked("tiraCmdThread")) {
      Sleep(10);
   }
              
   processClientCmd(tiraCmd->sz, tiraCmd);
   tiraCmdActive++;
   delete tiraCmd;
   unlock();
}   
 

// socket callback for smart remote clients 
void processClientCmd(const char * szCmd) {
   processClientCmd(szCmd, NULL);  // process message
}


// common function used by smart and ir remote callback routines
void processClientCmd(const char * szCmd, TiraCmd * tiraCmd) {

   struct rcCmd * cmd = cmdSzToRcCmd(szCmd);
   const char * szCmdArg = strlen(szCmd) > strlen(cmd->sz) ? &szCmd[strlen(cmd->sz) + 1] : NULL;
   static BOOL refreshEnable=TRUE, isTiraCmd=FALSE;
   int i, appId = getForegroundWindowAppID();
   
   if (tiraCmd) {
      isTiraCmd=TRUE;
      printf("%s", tiraCmd->debug);
      if (tiraCmd->duplicate || cmd->id==UNDEFINED)
         return;
   }      
   
   printf("processClientCmd: %s%s\n", szCmd, cmd->id==UNDEFINED ? " undefined" : "");
   
   switch (cmd->id) {
      case ARROW_LEFT:
         // disable surround sound or big/chapter jump back
         if (displayMode == DVR) {
            if (appId == NETFLIX) {
               // shift-left nine 10 second periods (90)
               sendVirtualKeyCode(KBD_LEFT_ARROW_SHIFT, 9);
            } else if (appId == VLC) {
               // jump back a chapter
               sendVirtualKeyCode(KBD_SHIFT_P, 1);
            } else if (appId == WMC) {             
               // ctrl-b thirteen 7 second periods (91)
               sendVirtualKeyCode(KBD_CTRL_B, 13);
            } else if (appId == YOUTUBE) {
               // left arrow eighteen 5 second periods (90) 
               sendVirtualKeyCode(KBD_LEFT_ARROW, 18);
            } else {
               setSurroundSound(OFF);
            }   
         }   
         else {   
            sendItachCmd(cmd->id, getRemoteMode());
         }   
         break;     
      case ARROW_RIGHT:
         // enable surround sound or big/chapter jump forward  
         if (displayMode == DVR) {
            if (appId == NETFLIX) {
               // jump forward nine 10 second periods (90)
               sendVirtualKeyCode(KBD_RIGHT_ARROW_SHIFT, 9);
            } else if (appId == VLC) {
               // jump forward a chapter
               sendVirtualKeyCode(KBD_SHIFT_N, 1);
            } else if (appId == WMC) {
               // ctrl-f three 30 second periods (90)
               sendVirtualKeyCode(KBD_CTRL_F, 3);
            } else if (appId == YOUTUBE) {
               // left arrow eighteen 5 second periods (90)
               sendVirtualKeyCode(KBD_RIGHT_ARROW, 18);
            } else {
               setSurroundSound(ON);
            }   
         }   
         else {   
            sendItachCmd(cmd->id, getRemoteMode());
         }   
         break;
      case ARROW_DOWN:
      case ARROW_UP:
         if (displayMode == DVD) {
            // DVD menu up and down
            sendDefault(cmd);
         } else {
            // DVR or TV mode   
            if (displayMode == DVR && isExplorerForeground()) {
               sendVirtualKeyCode(cmd->id == ARROW_UP ? KBD_ARROW_UP : KBD_ARROW_DOWN, 1);
               if (refreshEnable) {
                  printf("refresh command issued\n");
                  sendVirtualKeyCode(KBD_CTRL_R, 1);
                  refreshEnable=FALSE;
               }   
            }   
            else {  
               // streaming or live tv surround sound control
               setSurroundSound(cmd->id==ARROW_UP ? ON : OFF);
            }
         }    
         break;
      case EJECT:
         // eject regardless of mode because it only applies to a DVD
         sendItachCmd(EJECT, DVD_MODE);
         break;
      case LAUNCH_EXPLORER:
         launchExplorer(szCmdArg);
         break;   
      case MUTE:
         if (!szCmdArg) {
            (muteState==MUTE_ON ) ? sendArCmd(SZ_MUTE_OFF) : sendArCmd(SZ_MUTE_ON);
         } else if (strstr(szCmdArg, "off")) {
            sendArCmd(SZ_MUTE_OFF);
         } else if (strstr(szCmdArg, "on")) {
            sendArCmd(SZ_MUTE_ON);
         } 
         break;
      case VIDEO_POWER:
         if (!szCmdArg || 
             strstr(szCmdArg, "off") && tvPowered ||
             strstr(szCmdArg, "on") && !tvPowered) { 
            toggleVideoPower();
         }   
         break;
      case REWIND:
         if (appId == WMC) {             
            // ctrl-b four 7 second periods to approximate a 30 second CTRL_F
            sendVirtualKeyCode(KBD_CTRL_B, 2);
         } else {
            sendDefault(cmd);
         }
         break;   
      case SURROUND:
         setSurroundSound(strstr(szCmdArg, "on") ? ON : OFF);
         break;   
      case MODE:
         // set video and audio sources
         if (!isAudioFromVideo() && !szCmdArg) {
            // when switching from a non-video source and the requested source
            // is not explicitly specified, reset the audio to source from the
            // current display
            setAudioSource(VIDEO_AUX);   // pseudo mute   
            sendArCmd(SZ_FRONT_SPEAKERS_ON);
            sendArCmd(SZ_CHANNEL_B_OFF);
            sendArCmd(szDefaultMasterVolume);
            if (displayMode==UNDEFINED) {
               setVideoMode(TV);
               setAudioMode(TV);
            } else {
               setAudioMode(displayMode);
            }   
         } 
         else if (szCmdArg) {
            // switch to a requested video and audio source
            int argCmdId = cmdSzToRcCmdId(szCmdArg);
            if (argCmdId == TV || argCmdId == DVD || argCmdId == DVR) {
               setVideoMode(argCmdId);
               setAudioMode(argCmdId);
            } else {
               printf("error: invalid video mode %s\n", szCmdArg);
            }      
         }
         else {  
            // ir remote iterate round robin through tv, dvd, and dvr sources
            switch (nextMode) {
               case TV:
                  nextMode=DVD;
                  break;
               case DVD:
                  nextMode=DVR;
                  break;
               case DVR:
                  nextMode=TV;
                  break;
               default:
                  printf("error: invalid displayMode %s\n", cmdIdToRcCmd(nextMode)->sz);
                  break;
            }
            printf("displayMode: round robin %s\n", cmdIdToRcCmd(nextMode)->sz); 
            if (muteState==MUTE_OFF && displayMode==TV)
               sendArCmd(SZ_MUTE_ON);
            else
               menuFlashThreadStart();   // flash menu for ir remote feedback 
            videoModeChangeThreadStart(nextMode);
         }
         break;
      case VOLUME_UP:
         sendArCmd(SZ_MASTER_VOLUME_UP);
         break;
      case VOLUME_DOWN:
         sendArCmd(SZ_MASTER_VOLUME_DOWN);
         break;
      case MENU:
      case NEXT_WINDOW:
         if (displayMode==DVR) {
            refreshEnable=TRUE;
            setNextForegroundWindow();
         } else {
            sendItachCmd(cmd->id, getRemoteMode());
         }   
         break;
      case RESTORE_STATE:
         restoreReceiverState();
         break;
      case GET_STATE:
         sendStateInformation();
         break;
      case EXIT:
         printf("Exiting...\n");
         if (tvPowered) {
            toggleVideoPower();
            Sleep(6000);
         }   
         stty_buffered(); 
         shutdownServer();
         rxClientThreadExit(&arAgentSd);
         rxClientThreadExit(&itachSd);
         WSACleanup();
         unloadTiraDll();
         FreeLibrary(handle);
         
         if (ttyExit) {
            ttyExit=FALSE;
            DWORD count;
            WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), 
               inputQ, sizeof(inputQ)/sizeof(INPUT_RECORD), &count);
         } 
         
         if (!isConsoleMode()) exit(0);
         
         break;
      case PLAY:
         if (displayMode==DVR && appId==APP_UNKNOWN) {
            refreshEnable=TRUE;
            sendVirtualKeyCode(KBD_ENTER, 1);
            break;
         }
      default:
         sendDefault(cmd);
         break;
   }
}


void sendDefault(struct rcCmd * cmd) {
   if (displayMode==DVR)
      sendVirtualKeyCode(getVirtualKeyCode(cmd), 1);
   else
      sendItachCmd(cmd->id, getRemoteMode());
}


void toggleVideoPower() {
   const char * szState = tvPowered ? " OFF" : " ON";
   char szCmd[strlen(SZ_VIDEO_POWER) + strlen(szState) + 1];
   tvPowered = ~tvPowered;
   if (tvPowered) {
      sendArCmd(SZ_REGISTER);
      toggleDvdPower(displayMode);
   }   
   else {
      if (displayMode!=TV)
         setVideoMode(TV);
      else if (isAudioFromVideo()) 
         setAudioSource(VIDEO_AUX);   // pseudo mute   
         
      sendArCmd(SZ_DEREGISTER);
   }
   
   if (tvPowered)
      sendItachCmd(RETURN, TV_MODE);   // panasonic TV hack to reset ir receiver state
      
   sendItachCmd(VIDEO_POWER, TV_MODE, INDEPENDENT, tvPowered ? VIDEO_POWER_ON_DELAY : 0);
   
   sprintf(szCmd, "%s %s", SZ_VIDEO_POWER, szState);
   updateState(szCmd);
}   


void setSurroundSound(int state) {

   if (frontSpeakerState==FRONT_SPEAKERS_OFF) 
      sendArCmd(SZ_FRONT_SPEAKERS_ON);

   switch(state) {
      case ON:   
         sendArCmd(SZ_REAR_SPEAKERS_ON);
         break;
      case OFF:
         sendArCmd(SZ_REAR_SPEAKERS_OFF);
         break;
      default:   
         printf("setSurroundSound: unknown state %d\n", state);   
         break;
   }
}   

// set the audio source to the associated videoMode
void setAudioSource(int videoMode) {
   switch(videoMode) {
      case DVR:
         sendArCmd(getHostname());
         break;
      case TV:
      case DVD:
      case VIDEO_AUX:
         sendArCmd(tiraCmdIdToSz(videoMode));   
         break;
      default:
         printf("setAudioSource: unknown source %s\n", tiraCmdIdToSz(videoMode));   
         break;
   }      
}

// return TRUE if the current audioSource corresponds to a video mode
BOOL isAudioFromVideo() {
   switch (audioSource) {
      case TV:
      case DVD:
         return audioSource==displayMode;
      case LVR_MM:
      case DEN_MM:
         return displayMode==DVR;
      default:
         return FALSE;
   }      
}

// translate the given audioSource to the associated video mode
int getVideoMode(int audioSource) {    
   switch (audioSource) {
      case TV:
      case DVD:
         return audioSource;
      case LVR_MM:
      case DEN_MM:
         return DVR;
      default:
         return UNDEFINED;
   }      
}


void setAudioMode(int requestedMode) {
   printf("setAudioMode: %s\n", cmdIdToRcCmd(requestedMode)->sz); 
   setSurroundSound(requestedMode==DVD);
   setAudioSource(requestedMode);
}   


// set the audio/video source, the surround sound, and turn the 
// DVD player on in DVD mode
void setVideoMode(int requestedMode) {

   int previousMode=displayMode;
   printf("setVideoMode: %s\n", cmdIdToRcCmd(requestedMode)->sz); 
   
   toggleDvdPower(requestedMode);
   
   nextMode=displayMode=requestedMode;
   
   if (isAudioFromVideo()) 
      setAudioSource(VIDEO_AUX);   // pseudo mute   
   
   if (previousMode==DVD || requestedMode==DVD) {
      // delay 2000 milliseconds before and after sending the DEPENDENT video
      // MODE number for processing.  
      // tv: video+4 (hdmi switch input)
      // dvd: video+2 (dvd component input)
      // dvr: video+4 (hdmi switch input)
      sendItachCmd(MODE_MENU, TV_MODE, INDEPENDENT, 1500);
      sendItachCmd(requestedMode, TV_MODE, DEPENDENT, 4000);
   }
   
   // set the hdmi switch source   
   setVideoSource(requestedMode);
   
   if (previousMode != displayMode) {
      // notify clients
      char szCmd[strlen(SZ_MODE) + strlen(cmdIdToRcCmd(requestedMode)->sz) + 2];
      sprintf(szCmd, "%s %s", SZ_MODE, cmdIdToRcCmd(requestedMode)->sz);
      updateState(szCmd);
   }   
}


void toggleDvdPower(int requestedMode) {
   if (requestedMode==DVD && !dvdPowered || 
       requestedMode!=DVD && dvdPowered) {
      sendItachCmd(VIDEO_POWER, DVD_MODE);
      dvdPowered = ~dvdPowered;
   }
}


// send an ir command to set the hdmi switch source
void setVideoSource(int mode) {
   int videoSource;
   struct tiraCmd * videoSourceCmd;
   
   switch(mode) {
      case TV:
         videoSource=HDMI_SET_TOP_BOX_SOURCE;
         break;
      case DVD:      
         videoSource=HDMI_DVD_SOURCE;
         break;
      case DVR:   
         videoSource=HDMI_DVR_SOURCE;
         break;
      default:
         printf("setVideoSource: no source for mode %s\n", tiraCmdIdToSz(mode));   
         return;
   }
   
   videoSourceCmd = tiraCmdIdToTiraCmd(videoSource);
   
   if (*videoSourceCmd->state==videoSource)
      return;
   
   while (GetTickCount() - irRxTime < 550) {
      // try to avoid ir remote interference
      Sleep(10);
   }
      
   if (!tiraTransmit(videoSource)) {
      updateState(videoSourceCmd->sz);
      Sleep(4500);   // give the switch time to complete the request
   }   
}         


void menuFlashThreadStart() {
   HANDLE hThread;
   if (hThread=CreateThread(
         NULL,    // default security
         0,       // default stack size
         (LPTHREAD_START_ROUTINE) menuFlashThread,
         NULL,    // thread parameter
         0,       // run thread immediately
         NULL)    // ignore thread id
      ) {
      CloseHandle(hThread);
   } else {
      printf("menuFlashThreadStart failed\n");
   }   
} 


// flash a menu on the screen as feedback to a user that the system has queued 
// up an IR command. This routine was specifically written for user feedback 
// when iteratively toggling through video modes
DWORD menuFlashThread(LPVOID param) {
   static volatile LONG menu=FALSE;
   while (isLocked(&menu))          // sequence menu flashes
      Sleep(10);
   sendItachCmd(MENU, TV_MODE);     // display menu
   Sleep(2000);                     // allow user time to notice
   sendItachCmd(RETURN, TV_MODE);   // erase menu
   unlock(&menu);
   ExitThread(0);
}


void videoModeChangeThreadStart(int mode) {
   
   if (videoChange.hThread) {
      printf("VideoModeChangeThreadStart: terminating thread\n");
      TerminateThread(videoChange.hThread, 0);
      CloseHandle(videoChange.hThread);
   }
   
   videoChange.mode = mode;   
      
   if ((videoChange.hThread=CreateThread(
         NULL,      // default security
         0,         // default stack size
         (LPTHREAD_START_ROUTINE) videoModeChangeThread,
         NULL,      // thread parameter
         0,         // run thread immediately
         NULL))     // ignore thread id
      == NULL) {
      printf("videoModeChangeThreadStart failed\n");
   }
}


DWORD videoModeChangeThread(LPVOID param) {
   // give user time to iterate with ir remote to the desired mode
   Sleep(4000);
   CloseHandle(videoChange.hThread);
   videoChange.hThread = NULL;
   while (isLocked("videoModeChangeThread"))
      Sleep(10);
   setVideoMode(videoChange.mode);
   setAudioMode(videoChange.mode);
   unlock();
   ExitThread(0);
}


// queue an itach command retry thread with the ItachCmdParam parameters used 
// to verify success of the requested itach IR command
void sendItachCmdThreadStart(ItachCmdParam * retry) {
   HANDLE hThread;
   
   if (hThread=CreateThread(
         NULL,  // default security
         0,     // default stack size
         (LPTHREAD_START_ROUTINE) sendItachCmdThread,
         retry, // thread parameter
         0,     // run thread immediately
         NULL)  // ignore thread id
      ) {
      CloseHandle(hThread);
   } else {
      printf("sendItachCmdThread create failed\n");
   }   
   
}


// pace, serialize and confirm success of itach command requests
DWORD sendItachCmdThread(LPVOID param) {
   ItachCmdParam * itachCmdParam = (ItachCmdParam *) param;
   const char * szCode = itachCmdParam->irTx->code;
   const char * szDesc = itachCmdParam->irTx->sz;
   struct irRx * irRx = itachCmdParam->irRx;
   int sequence = itachCmdParam->sequence;
   int dependent=itachCmdParam->dependent;
   int delay=itachCmdParam->delay;
   HANDLE hThread;
   int retry, retryMax=3, replyWait;
   
   for (retry=0; retry<retryMax; retry++) {
      
      while (sequence!=itachCmd.sequence ||         // sequence itach messaging
             GetTickCount() - irRxTime < 550 ||     // avoid ir remote interference
             isLocked("sendItachCmdThread")) {      // serialize msg processing
         Sleep(10);
      }
         
      if (dependent && itachCmd.prerequisiteFailure) {
         // bail because the prerequisite sendItachCmd failed
         printf("sendItachCmdThread: exiting dependent sendItachCmd retry thread %s\n", szDesc);
         unlock();
         delete itachCmdParam;
         ExitThread(0);
      }
      
      if (retry) {
         printf("sendItachCmdThread attempting retry %d: %s\n", retry, szDesc);
         printf("   irRxPending: %d && irComplete: %d\n", 
            itachCmd.irRxPending!=NULL, itachCmd.irComplete);
      } 
         
      if (itachSocket) {
         itachCmd.irRxPending=irRx;
         itachCmd.irComplete=FALSE;
         
         // try to recover with an immediate resend if itach connection lost
         setReconnectMsg(&itachSd, szCode, szDesc);   
         
         sendMsg(&itachSd, szCode, szDesc);
      }   
         
      unlock();
      
      if (delay) {
         // allow itach commands time to process 
         // e.g. tv power on, video menu pop up, etc
         Sleep(delay);      
      }   
         
      for (replyWait=0; 
          !(!itachCmd.irRxPending && itachCmd.irComplete || replyWait>1500); 
          replyWait+=10) {
         // wait for the requested ir code and a "completeir" tcp message from the
         // itach or a timeout
         Sleep(10);
      }
      
      if (!itachCmd.irRxPending && itachCmd.irComplete)
         break;
   }
   
   while (isLocked("sendItachCmdThread 2"))
      Sleep(10);
         
   if (!itachCmd.irRxPending && itachCmd.irComplete) {
      // irRxCmdCallback received the expected IR code AND
      // a "completeir" tcp message was received from the itach
      printf("%s", itachCmd.debug);
      printf("sendItachCmdThread success: %s\n\n", szDesc); 
   }
   else {    
      printf("sendItachCmdThread failure: %s\n", szDesc); 
      printf("   irRxPending: %d && irComplete: %d\n", itachCmd.irRxPending!=NULL, itachCmd.irComplete);
      if (!dependent)
         itachCmd.prerequisiteFailure = TRUE;
   }   
   clrReconnectMsg(&itachSd);
   itachCmd.sequence++;  // move on to process the next itach message
   unlock();
   delete itachCmdParam;
   ExitThread(0);
}

    
// return the ID of the foreground window to perform context sensitive
// operations (e.g. jump forward/back, turn dvd player on, etc.)
int getForegroundWindowAppID() {

   int i;
   DWORD pid;
   HWND hWnd;
   char windowTitle[1024];
   
   if ((hWnd = GetForegroundWindow())!=NULL) {
      GetWindowText(hWnd, windowTitle, sizeof(windowTitle));
      for (i=0; i<sizeof(appTitle)/sizeof(struct appTitle); i++) {
         if (strstr(windowTitle, appTitle[i].sz))
            return appTitle[i].id;
      }      
   } 
   else {
      printf("\nError: getForegroundWindow: window handle not found\n");
   }       
   
   return APP_UNKNOWN;
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


// setNextForegroundWindow uses GetNextWindow and GetWindowText to 
// maintain a list of windows that partially match an entry in the 
// titleList array.  After updating the list the next round-robin
// entry in the list is used to call SetForegroundWindow.  If no windows
// with partially matching titles exist, SetForegroundWindow is not called.
//
// Although titleList can contain any arbitrary window title, the intended
// use is to iterate over explorer windows.  In order to differentiate
// explorer windows, it is important for the title to fully qualify the
// current path.  In any explorer window, go to Organize/Folder Options/View 
// and check "Display the full path in the title bar (Classic theme only)"

void setNextForegroundWindow() {
   unsigned i, titleLen;
   char title[1024];
   char titleFG[1024];
   char * titleNext;
   const char * result = "";
   HWND hWndNext = NULL;
   HWND hWnd = GetTopWindow(NULL);
   HWND hWndFG = GetForegroundWindow();
   GetWindowText(hWndFG, titleFG, sizeof(titleFG));
   static LinkedList * dwl = NULL;
   
   struct winInfo {
      char title[1024];
      HWND hWnd;
      unsigned count;  // number of windows partially matching an entry in titleList
   } * wi;
   
   const char * titles[] = {
      "I:\\Recorded TV",
      "I:\\home\\jhm\\video\\lnk"
   };
   
   if (!dwl)
      dwl = new LinkedList();  // desktop window list
   
   for (dwl=dwl->first(); dwl->isNotLast(); dwl=dwl->next())
      ((winInfo *)dwl->get())->count=0;
   
   // iterate through every desktop window.  If a titles entry
   // partially matches a window's title, save the window's handle
   // to a windowList struct and increment the count.  This loop 
   // effectively refreshes the current list of desktop windows
   while(hWnd) {
      titleLen = GetWindowText(hWnd, title, sizeof(title));
      for (i=0; titleLen>0 && i<sizeof(titles)/sizeof(char *); i++) {
         if (strstr(title,titles[i])) {
            // this window's title matches or is a subdirectory of titles[i]
            for (dwl=dwl->first(); dwl->isNotLast(); dwl=dwl->next()) {
               wi=(struct winInfo *)dwl->get();
               if (strstr(wi->title, title)) {
                  // this window title exists in the window list
                  break;
               }
            }
            if (dwl->isLast()) {
               // window title not found in the window list
               wi = new struct winInfo;
               memset(wi,'\0',sizeof(struct winInfo));
               dwl->add(wi);
               strcpy(wi->title, title);
            }
            wi->count++;
            wi->hWnd = hWnd;
         }
      }            
      hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
   }

   // now iterate through the list of desktop windows to find the next matching
   // window handle
   for (dwl=dwl->first(); dwl->isNotLast() && hWndNext==NULL; dwl=dwl->next()) {
      wi=(struct winInfo *)dwl->get();
      if (strstr(wi->title, titleFG)) {
         LinkedList * dwn;
         for (dwn=dwl->nextWrap(); dwn!=dwl; dwn=dwn->nextWrap()) {
            wi=(struct winInfo *)dwn->get();
            if (wi->count) {
               hWndNext = wi->hWnd;
               titleNext = wi->title;
               break;
            }
         }
      }
   }

   if (hWndNext==NULL &&
       dwl->first()->isNotLast()) {
      wi=(struct winInfo *)dwl->first()->get();
      if (wi->count &&
          hWndFG != wi->hWnd) {
         // a next window was not found because the current foreground window 
         // does not match anything in titles so set the next handle to the 
         // first window in the list
         hWndNext = wi->hWnd;
         titleNext = wi->title;
      }
   }
         
   if (hWndNext)
      result = forceForegroundWindow(hWndNext) ? "success" : "fail";
   
   printf("setNextForegroundWindow: %s %s\n", hWndNext ? titleNext : "not found", result);
}


// trick the windows desktop into thinking our application has foreground 
// window privilege
BOOL forceForegroundWindow(HWND hWnd) {
   DWORD foreThread, appThread;

   foreThread = GetWindowThreadProcessId(GetForegroundWindow(), 0);
   appThread = GetCurrentThreadId();

   if (foreThread != appThread) {
       AttachThreadInput(foreThread, appThread, TRUE);
       BringWindowToTop(hWnd);
       ShowWindow(hWnd,SW_SHOWNORMAL);
       AttachThreadInput(foreThread, appThread, FALSE);
   } else {
       BringWindowToTop(hWnd);
       ShowWindow(hWnd,SW_SHOWNORMAL);
   }

   return SetForegroundWindow(hWnd); 
}


// launch an explorer window with the given path
// if the window to the given path exists, bring it to the foreground
void launchExplorer(const char * path) {
   HWND hWnd;

   if (!path) {
      printf("launchExplorer: no path specified\n");
      return;
   }   
   else if (hWnd=findWindow(path)) {
      forceForegroundWindow(hWnd);
   }
   else {
      const char * explorer = "\\explorer.exe";
      char * systemRoot = getenv("SYSTEMROOT");
      if (systemRoot==NULL) {
         printf("launchExplorer: Error SYSTEMROOT environment variable not defined\n");
         return;
      }   
      char exe[strlen(systemRoot) + strlen(explorer) + 1];
      strcat(strcpy(exe, systemRoot), explorer); 
      createProcess(exe, path);
   }
}

// return TRUE if the windows explorer program is running in the foreground
BOOL isExplorerForeground() {
   char szProcessName[MAX_PATH];
   const char * szPattern = "\\windows\\explorer.exe";
   getForegroundWindowImageFileName(szProcessName);
   char * szMatch = strstr(szToLower(szProcessName), szPattern);
   return (szMatch && strlen(szMatch)==strlen(szPattern));
}   


// return the file name of the executable running in the foreground
// note: sizeof the szProcessName argument must be MAX_PATH or greater 
char * getForegroundWindowImageFileName(char * szProcessName) {

   HWND hWnd;
   DWORD pid;
   HANDLE hProcess;

   if (!( (hWnd = GetForegroundWindow()) &&
          (GetWindowThreadProcessId(hWnd, &pid)) &&
          (hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid)) &&
          (GetProcessImageFileName(hProcess, szProcessName, MAX_PATH)) )) {
          strcpy(szProcessName, "<unknown>");
   }

   if (hProcess)
      CloseHandle(hProcess);

   return szProcessName;
}


// return the rcCmd id associated to a command string
int cmdSzToRcCmdId(const char * msg) {
   int i;

   // search for an exact match or a match with an argument
   for (i=0; i<sizeof(rcCmd)/sizeof(struct rcCmd); i++) {
      if (strstr(msg, rcCmd[i].sz)==msg && strlen(msg) >= strlen(rcCmd[i].sz))
         return rcCmd[i].id;
   }

   return UNDEFINED;
}


// use a for loop to call the sendVirtualKeyCode function count times   
void sendVirtualKeyCode(unsigned vkCode, int count) {
   char szCount[32];
   int i;
   snprintf(szCount, sizeof(szCount), " %dx", count);
   printf("sendVirtualKeyCode: %s%s\n", vkCodeToSz(vkCode), count>1 ? szCount:"");
   for (i=0; i<count; i++)
      sendVirtualKeyCode(vkCode);
}      
   

// do the legwork to press and release a keyboard key using the win32 
// SendInput function
void sendVirtualKeyCode(unsigned vkPress) {
   if (!vkPress)
      return;
   
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


// indexed to the current foreground window and return the shortcut keycode
int getVirtualKeyCode(struct rcCmd * cmd) {
   int winID = getForegroundWindowAppID();
   
   if (!cmd->shortcut || winID==APP_UNKNOWN && cmd->shortcut->vkCode[1]) {
      // no shortcut OR foreground application unknown for rcCmd's
      // indexed shortcut
      return 0;
   }   
   else if (winID!=APP_UNKNOWN && cmd->shortcut->vkCode[winID]) {
      // indexed shortcut
      //printf("getVirtualKeyCode: %s\n", cmd->shortcut->sz[winID]); 
      return cmd->shortcut->vkCode[winID];
   }   
   else {
      // universal shortcut
      //printf("getVirtualKeyCode: %s\n", cmd->shortcut->sz[0]); 
      return cmd->shortcut->vkCode[0];
   }   
}


// return the rcCmd structure associated to a command string
struct rcCmd * cmdSzToRcCmd(const char * msg) {
   int i;

   // search for an exact match or a match with an argument
   for (i=0; i<sizeof(rcCmd)/sizeof(struct rcCmd); i++) {
      if (strstr(msg, rcCmd[i].sz)==msg && strlen(msg) >= strlen(rcCmd[i].sz))
         return &rcCmd[i];
   }

   return &rcCmd[UNDEFINED];
}


// return the rcCmd structure associated to a command id
struct rcCmd * cmdIdToRcCmd(int id) {
   int i;
   for (i=0; i<sizeof(rcCmd)/sizeof(struct rcCmd); i++) {
      if (id==rcCmd[i].id)
         return &rcCmd[i];
   }
   return &rcCmd[UNDEFINED];
}


// send an ir command via the usb attached tira device
int tiraTransmit(int cmd) {

   int result=0;
   
   printf("Transmitting IR command: %s\n", tiraCmdIdToSz(cmd));
   
   struct tiraCmd * tiraCmd = tiraCmdIdToTiraCmd(cmd);
   
   if (tiraCmd->codes) {
      result = tiraTransmit(tiraCmd);
   } else {
      printf("tiraTransmit: No IR code for IR command: %s\n", tiraCmdIdToSz(cmd));
      result = -1;
   }

   if (!result)
      setState(tiraCmd->sz);
       
   return result;
}


// return the virtual key code associated with a command string
const char * vkCodeToSz(unsigned vkCode) {
   int i,j;
   for (i=0; i<sizeof(rcCmd)/sizeof(struct rcCmd); i++) {
      if (rcCmd[i].shortcut) {
         shortcut * sc = rcCmd[i].shortcut;
         for (j=0; j<APP_SIZE; j++) {
            if (sc->vkCode[j]==vkCode)
               return sc->sz[j];
         }
      }         
   }
   return SZ_UNDEFINED;
}   


// return the ir remote mode to provide context aware functionality
// This function simulates an ir remote's TV and DVD mode buttons 
int getRemoteMode() {
   switch (displayMode) {
      case TV:
         return TV_MODE;
      case DVD:
         return DVD_MODE;
      default:
         return UNDEFINED_MODE;
   }
}   


void help() {
   printf( "\ntype \'q\' to quit\n");
}





