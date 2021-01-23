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
// TV Power      -> Toggle Amplifier On; Init Matrix/Set Audio Source; Init Volume; TV On
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
// 2009 Apr 10 jhm added IR cmd translations for videolan media player
// 2013 Apr 28 jhm added IR cmd translations for windows media center
// 2015 Dec 24 jhm added a thread to listen for audio control notifications
// 2016 Jul 17 jhm added smart remote functionality
// 2018 Sep 04 jhm added zigbee remote control functionality
//                 replaced spinlocks with message queues for thread
//                 coordination and command serialization


// SendInput() is only declared for Windows XP and later
#define _WIN32_WINNT 0x0501

#include <winsock2.h>
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include "common.h"
#include "socket.h"
#include "tira_dll.h"
#include "rcAgent.h"
#include "zrc.h"

int main(int argc, char ** argv) {

   processArgs(argc, argv);
   
   HANDLE hThread = initExitHandler();
   itachCmd = new ItachCmd(); 
   initConsoleModeExitHandler();
   
   initTiraRemoteControl();
   initZigbeeRemoteControl();
   processClientMsgThreadStart();
   initWinsock();
   initClientThread(&arAgentSd);
   initClientThread(&itachSd);
   initTcpServer(getHostname(), REMOTE_CONTROL_PORT);
   
   isConsoleMode() ?
      ttyLoop() :
      WaitForSingleObject(hThread,INFINITE);
}


// initialize the HOMElectronics Tira2 IR receiver/transmitter and callback
int initTiraRemoteControl() {
   int error;
   
   initTiraRemoteControl(DEFAULT_TIRA_COM_PORT);
  
   printf("Tira set callback handler %s\n", 
          (error=tira_set_handler(irRxCmdCallback)) ? "failed" : "succeeded");
          
   if (error) exit(1);
   
   return 0;
} 

// initialize the Texas Instruments CC2531EMK zigbee remote control usb dongle
void initZigbeeRemoteControl() { 
   initZigbeeRemoteControl(DEFAULT_ZRC_COM_PORT); 
}


// tira callback for ir remote clients (e.g. itach, universal remote, etc.)
int __stdcall irRxCmdCallback(char const * irCode) {

   DWORD discardDuplicatePeriod, rxDelta;
   int duplicate=FALSE, noise=FALSE;
   char irRxDebug[256], szQueuedDelta[32];
   const char * result;
   static int queuedCmdId;
   
   szQueuedDelta[0] = '\0';
   struct rcCmd * cmd = irRxCmdToRcCmd(irCode);
   irRxTime=GetTickCount();
   
   if (cmd->id == UNDEFINED) {
      result = "match not found";
   }
   else {
      discardDuplicatePeriod = cmd->id==VIDEO_POWER ? 5000 : IR_RX_QUIET_PERIOD;
      duplicate = irRxTime - cmd->irRx->timestamp < discardDuplicatePeriod;  
      result = duplicate ? "discarding duplicate" : "match found";
      
      if (irRxTime - queuedIrRxTime < IR_RX_NOISE_PERIOD &&
          queuedCmdId != cmd->id &&
          !(duplicate || cmd->id == UNDEFINED || cmd->id == IR_IGNORE || debug)) {
         // discard this command as noise because it was received faster than
         // a user can press two different keys
         // duplicate/undefined/ignored/debug cases are handled independently
         // unique commands in rapid succession are likey due to plasma tv ir noise
         noise = TRUE;
         result = "discarding noise";
         snprintf(szQueuedDelta, sizeof(szQueuedDelta), "(%d)", 
                  irRxTime - queuedIrRxTime);
      } 
      
      // timestamp receipt to discard subsequent IR commands repeated from
      // a user holding down a remote key or an itach command coded to repeat   
      cmd->irRx->timestamp=irRxTime;
   }
   
   snprintf(irRxDebug,  
      sizeof(irRxDebug),
      "irRx %s %s%s: %s -> %s\n",
      result,
      szTime(),
      szQueuedDelta,
      irCode, 
      cmd->irRx->sz);
      
   if (irRxHasCode(itachCmd->irRxPending, irCode)) {
      // irCode from the itach exists in the array of expected irRx codes 
      // set irRxPending=NULL to notify the processClientMsgThread's
      // processItachCmd waiting in a spin loop the expected command was received
      strncpy(itachCmd->debug, irRxDebug, sizeof(itachCmd->debug)); 
      itachCmd->irRxPending = NULL;
      return 0;
   }   
   
   if (duplicate || noise || cmd->id == UNDEFINED || debug) {
      // log but do not process duplicate/noise/undefined commands
      // do not process any commands when started with the -d debug option for
      // defining and validating IR callback codes in tira_rx.h
      debug ? printf("%s", irRxDebug) : printq("%s", irRxDebug);
   }   
   else if (cmd->id != IR_IGNORE) {   
      // ignore ir commands not meant for this agent.  e.g. because
      // no vcr exists then all vcr commands are really meant for this agent 
      // tv and dvd commands are meant for existing equipment
      // add this tira cmd to the message queue for sequential processing
      queuedIrRxTime = irRxTime;
      queuedCmdId = cmd->id;
      msgQueueAdd(cmd->sz, new IrCmd(irRxDebug, cmd->id, irRxTime));
   }
   
   return 0;
}


//// begin agent specific socket hooks
void deregisterClient(struct socketDescriptor * sd) {}
void processServerConnect(const char * agent) {
   if (!strcmp(agent, arAgentSd.host) && tvPowered)
      sendArCmd(SZ_REREGISTER);
}

// handle arAgent and itach server messages
void processServerMsg(const char * msg, struct socketDescriptor * sd) {
   if (!strcmp(sd->host, arAgentSd.host))
      msgQueueAdd(msg, sd);
   else {
      printq("processServerMsg from %s: %s\n", sd->host, msg);
      if (strstr(msg, "completeir"))
         itachCmd->irComplete=TRUE;
   }      
}
//// end agent specific socket hooks


// send a TCP/IP command to the audio receiver agent
void sendArCmd(const char * szCmd) {
   if (arAgentSd.socket!=INVALID_SOCKET) 
      sendMsg(&arAgentSd, szCmd);
}


// TCP/IP and IR message processing thread   
DWORD processClientMsgThread(LPVOID param) {
   QueuedMsg * msg;
   
   while(!exiting) {
      while( !(msg=msgQueueRemove()) ) {
         printConsoleLogMsgs();   
         Sleep(100);
      }
     
      printConsoleLogMsgs();   
      if (msg->sd==&arAgentSd)
         processArAgentNotification(msg->sz);
      else   
         processClientCmd(msg); 
      delete msg;
   }
   
   isClientMsgThreadRunning=FALSE;
   printf("processClientMsgThread exiting\n");
   ExitThread(0);
}   
   

// update state information received from the audio receiver agent
// audio state information is used to initialize this agent at startup
// and after sending a registration request to the audio receiver agent
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
            const char * szVideoMode = cmdIdToRcCmd(getVideoMode(cmdId))->sz;
            char * szCmd = (char *) malloc(strlen(SZ_AR_MODE) + strlen(szVideoMode) + 2);
            sprintf(szCmd, "%s %s", SZ_AR_MODE, szVideoMode);
            msgQueueAdd(szCmd);
            free(szCmd);
         }   
         break;      
      case FIRST_REGISTRATION:
         // audio receiver agent's only client so ok to configure
         msgQueueAdd(SZ_AR_FIRST_REGISTRATION);
         break;
      case RESTORE_STATE:
         restoreReceiverState();   
         break;
   }
   
}


// process commands from smart clients and ir remotes
void processClientCmd(QueuedMsg * msg) {
   const char * szCmd = msg->sz;
   IrCmd * irCmd = msg->irCmd;
   BOOL isZigbeeAsleep = isZrcCmdQueueEmpty(); 
   BOOL isMsgChannelRequest = atoi(szCmd) && !irCmd && displayMode==TV;
   struct rcCmd * cmd = cmdSzToRcCmd(isMsgChannelRequest ? SZ_CHANNEL : szCmd);
   const char * szCmdArg = strlen(szCmd) > strlen(cmd->sz) ? &szCmd[strlen(cmd->sz) + 1] : NULL;
   static BOOL refreshEnable=TRUE, tvGuide=FALSE;
   int i, appId = getForegroundWindowAppID();
   
   const char * channelPrefix = cmd->id==CHANNEL ? "channel " : "";
   const char * undefinedSuffix = cmd->id==UNDEFINED ? " undefined" : "";
   printf("\nprocessClientCmd: %s%s%s\n", channelPrefix, szCmd, undefinedSuffix);
      
   if (irCmd)
      printf("%s", irCmd->debug);
   
   switch (cmd->id) {
      case AR_FIRST_REGISTRATION:
         if (displayMode==UNDEFINED) {
            sendArCmd(SZ_FRONT_SPEAKERS_ON);
            sendArCmd(szDefaultMasterVolume);
            setVideoMode(DVR);    // force initialization to a known state
            setAudioMode(DVR);
         }   
         else {   
            setSurroundSound(displayMode==DVD);
            sendArCmd(SZ_FRONT_SPEAKERS_ON);
            sendArCmd(szDefaultMasterVolume);
            setAudioSource(displayMode);
         }   
         break;
      case AR_MODE:
         if (displayMode==UNDEFINED && tvPowered) { 
            nextMode=displayMode=cmdSzToRcCmdId(szCmdArg);
            // initialize the current displayMode according to a registered 
            // client's audio state by setting the DVR input 
            //  tv: video input menu+4 (source hdmi switch)
            // dvd: video input menu+2 (source dvd)
            // dvr: video input menu+4 (source hdmi switch)
            sendItachCmd(VIDEO_INPUT_MENU, TV_MODE, INDEPENDENT, 1500);
            sendItachCmd(displayMode, TV_MODE, DEPENDENT, 4000);   
            
            // and the hdmi switch source
            setVideoSource(displayMode);
         }
         break;   
      case ARROW_LEFT:
         // disable surround sound or big/chapter jump back
         if (displayMode == DVR) {
            switch(appId) {
               case NETFLIX:
                  // shift-left nine 10 second periods (90)
                  sendVirtualKeyCode(KBD_LEFT_ARROW_SHIFT, 9);
                  break;
               case VLC:   
                  // jump back a chapter
                  sendVirtualKeyCode(KBD_SHIFT_P, 1);
                  break;
               case WMC:
                  // ctrl-b thirteen 7 second periods (91)
                  sendVirtualKeyCode(KBD_CTRL_B, 13);
                  break;
               case YOUTUBE:   
                  // left arrow eighteen 5 second periods (90) 
                  sendVirtualKeyCode(KBD_LEFT_ARROW, 18);
                  break;
               default:   
                  setSurroundSound(OFF);
            }   
         }
         else if (displayMode == TV)
            zrcCmdQueueAdd(cmd->zrc);
         else
            sendItachCmd(cmd->id, getRemoteMode());
         break;     
      case ARROW_RIGHT:
         // enable surround sound or big/chapter jump forward  
         if (displayMode == DVR) {
            switch(appId) {
               case NETFLIX:
                  // jump forward nine 10 second periods (90)
                  sendVirtualKeyCode(KBD_RIGHT_ARROW_SHIFT, 9);
                  break;
               case VLC:
                  // jump forward a chapter
                  sendVirtualKeyCode(KBD_SHIFT_N, 1);
                  break;
               case WMC:
                  // ctrl-f three 30 second periods (90)
                  sendVirtualKeyCode(KBD_CTRL_F, 3);
                  break;
               case YOUTUBE:
                  // right arrow eighteen 5 second periods (90)
                  sendVirtualKeyCode(KBD_RIGHT_ARROW, 18);
                  break;
               default:   
                  setSurroundSound(ON);
            }   
         }   
         else if (displayMode == TV)
            zrcCmdQueueAdd(cmd->zrc);
         else  
            sendItachCmd(cmd->id, getRemoteMode());
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
               if(tvGuide && displayMode==TV)
                  zrcCmdQueueAdd(cmd->zrc);
               else {   
                  // streaming or live tv surround sound control
                  setSurroundSound(cmd->id==ARROW_UP ? ON : OFF);
               }   
            }
         }    
         break;
      case CHANNEL:   
         sendZigbeeChannelRequest(szCmd);
         break;
      case EJECT:
         // eject regardless of mode because it only applies to a DVD
         sendItachCmd(EJECT, DVD_MODE);
         break;
      case EXIT_PAGE:
         if (displayMode==TV)
            tvGuide=FALSE;   
         sendDefault(cmd);
         break;
      case FEEDBACK_TV_MENU:   
         sendItachCmd(MENU, TV_MODE);     // display tv menu to inform user command queued
         break;
      case FEEDBACK_TV_RETURN:   
         sendItachCmd(RETURN, TV_MODE);   // erase tv menu used to inform user command queued
         break;
      case GET_STATE:
         sendStateInformation(msg);
         break;
      case GUIDE:
         if (displayMode==TV)
            tvGuide=TRUE;
         sendDefault(cmd);
         break;
      case LAUNCH_EXPLORER:
         launchExplorer(szCmdArg);
         break;   
      case MENU:
         static DWORD irRxMenuTime;
         switch (displayMode) {
            case TV:
               tvGuide=TRUE;
               sendDefault(cmd);
               break;
            case DVR:
               refreshEnable=TRUE;
               setNextForegroundWindow();
               break;
             case DVD:
               if (irCmd) {
                  if (queuedIrRxTime - irRxMenuTime < 10000) {
                     // universal remote DVD eject hack (no eject button on remote)
                     // user pressed menu button twice within 10 seconds
                     sendItachCmd(EJECT, DVD_MODE);
                     break;
                  } 
                  irRxMenuTime=queuedIrRxTime;
               }
            default:   
               sendDefault(cmd);
         }   
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
               setVideoMode(DVR);
               setAudioMode(DVR);
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
                  //nextMode=TV;
                  nextMode=DVD;
                  break;
               default:
                  printf("error: invalid displayMode %s\n", cmdIdToRcCmd(nextMode)->sz);
                  break;
            }
            printf("displayMode: round robin %s\n", cmdIdToRcCmd(nextMode)->sz); 
            int mute = muteState==MUTE_OFF && displayMode==TV;
            
            if (mute)
               sendArCmd(SZ_MUTE_ON);
               
            videoModeChangeThreadStart(!mute, nextMode);
         }
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
      case NEXT_WINDOW:
         if (displayMode==DVR) {
            refreshEnable=TRUE;
            setNextForegroundWindow();
         } 
         break;
      case OK:
         if (displayMode==TV)
            tvGuide=FALSE;   
         sendDefault(cmd);
         break;
      case PLAY:
         if (displayMode==DVR) {
            if (appId==APP_UNKNOWN) {
               // explorer window or desktop icon
               refreshEnable=TRUE;
               sendVirtualKeyCode(KBD_ENTER, 1);
               break;
            } else if (irCmd) {
               // disable play/pause toggle when a DVR video is playing
               //
               // plasma tv ir noise can cause a fast forward commmand to get 
               // captured as a play/pause toggle command because the current
               // set of definitions only differ by one bit:
               // vcrFastForward: 0x1C16
               //        vcrPlay: 0x1E16 
               //
               // ultimately need to eliminate plasma tv generated ir noise
               // this problem does not occur when plasma tv display is off
               printf("Notice: ir play command disabled\n");
               break;
            }
         }      
         sendDefault(cmd);
         break;
      case RESTORE_STATE:
         restoreReceiverState();
         break;
      case REWIND:
         if (appId == WMC) {             
            // ctrl-b four 7 second periods to approximate a 30 second CTRL_F
            sendVirtualKeyCode(KBD_CTRL_B, 2);
         } else {
            sendDefault(cmd);
         }
         break;   
      case STOP:
         if (displayMode==TV)
            tvGuide=FALSE;   
         sendDefault(cmd);
         break;
      case SURROUND:
         setSurroundSound(strstr(szCmdArg, "on") ? ON : OFF);
         break;   
      case VIDEO_POWER:
         if (!szCmdArg || 
             strstr(szCmdArg, "off") && tvPowered ||
             strstr(szCmdArg, "on") && !tvPowered) { 
            toggleVideoPower();
         }   
         break;
      case VOLUME_UP:
         sendArCmd(SZ_MASTER_VOLUME_UP);
         break;
      case VOLUME_DOWN:
         sendArCmd(SZ_MASTER_VOLUME_DOWN);
         break;
      case EXIT_PROCESS:
         if (exiting) {
            printf("Exit previously requested, forcing exit\n");
            exit(0);
         }
            
         printf("Exiting...\n");
         exiting=TRUE;
         
         if (tvPowered) {
            toggleVideoPower();
            Sleep(6000);
         }
         
         rfClose();   
         shutdownServer();
         rxClientThreadExit(&arAgentSd);
         rxClientThreadExit(&itachSd);
         
         while(areSocketThreadsRunning())
            Sleep(500); 
         
         WSACleanup();
         unloadTiraDll();
         printConsoleLogMsgs();   
         printf("Exit complete\n");
         
         if (isConsoleMode())
            tty_buffering(TRUE);
            
         exit(0);
      default:
         sendDefault(cmd);
         break;
   }
   
   if (isZigbeeAsleep && !isZrcCmdQueueEmpty()) {
      wakeupRNP();
      do Sleep(100); while (!isZrcCmdQueueEmpty());
   }   
}


// translate and send an ir remote or smart client command based on the 
// active mode (tv, dvd, dvr)
// e.g. a generic pause command will translate differently for live tv,
// a dvd, or a media player application 
void sendDefault(struct rcCmd * cmd) {
   if (displayMode==DVR && getVirtualKeyCode(cmd))
      sendVirtualKeyCode(getVirtualKeyCode(cmd), 1);
   else if (displayMode==TV && cmd->zrc)
      zrcCmdQueueAdd(cmd->zrc);
   else
      sendItachCmd(cmd->id, getRemoteMode());
}


void toggleVideoPower() {
   const char * szState = tvPowered ? " OFF" : " ON";
   char * szCmd = (char *) malloc(strlen(SZ_VIDEO_POWER) + strlen(szState) + 2);
   tvPowered = ~tvPowered;
   if (tvPowered) {
      sendArCmd(SZ_REGISTER);
      setDvdPower(displayMode);
   }   
   else {
      if (displayMode!=DVR)
         setVideoMode(DVR);
      else if (isAudioFromVideo()) 
         setAudioSource(VIDEO_AUX);   // pseudo mute   
         
      sendArCmd(SZ_DEREGISTER);
   }
   
   sendItachCmd(VIDEO_POWER, TV_MODE, INDEPENDENT, tvPowered ? VIDEO_POWER_ON_DELAY : 0);
   
   sprintf(szCmd, "%s %s", SZ_VIDEO_POWER, szState);
   updateState(szCmd);
   free(szCmd);
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


// set the audio source and enable surround sound for a DVD
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
   
   if (isAudioFromVideo()) 
      setAudioSource(VIDEO_AUX);   // pseudo mute   
   
   setDvdPower(requestedMode);
   
   nextMode=displayMode=requestedMode;
   
   if (previousMode==DVD || requestedMode==DVD) {
      setSpeakerVolume(SZ_CENTER_VOLUME,
                       requestedMode==DVD ? SZ_DEFAULT_CENTER_VOLUME_DVD :
                                            SZ_DEFAULT_CENTER_VOLUME);
      // change the TV's input source                               
      // delay values give the TV time to bring up the menu and commit the change
      //  tv: video input menu+4 (source hdmi switch)
      // dvd: video input menu+2 (source dvd)
      // dvr: video input menu+4 (source hdmi switch)
      //sendItachCmd(VIDEO_INPUT_MENU, TV_MODE, INDEPENDENT, 1500);
      //sendItachCmd(requestedMode, TV_MODE, DEPENDENT, 4000);
   }
   
   // set the hdmi switch source   
   setVideoSource(requestedMode);
   
   if (previousMode != displayMode) {
      // notify clients
      char * szCmd = (char *) malloc(strlen(SZ_MODE) + strlen(cmdIdToRcCmd(requestedMode)->sz) + 2);
      sprintf(szCmd, "%s %s", SZ_MODE, cmdIdToRcCmd(requestedMode)->sz);
      updateState(szCmd);
      free(szCmd);
   }   
}


// toggle dvd power if activating or deactivating dvd mode 
void setDvdPower(int requestedMode) {
   if (displayMode==DVD || requestedMode==DVD) { 
      sendItachCmd(VIDEO_POWER, DVD_MODE);
      dvdPowered = ~dvdPowered;
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


void setSpeakerVolume(const char * prefix, const char * level) {
   static int size=0;
   static char * cmd = NULL;
   if (size < strlen(prefix) + strlen(level) + 1) {
      size = strlen(prefix) + strlen(level) + 1;
      if (cmd)
         free(cmd);
      cmd = (char *) malloc(size);
   }
   sendArCmd(strcat(strcpy(cmd, prefix), level));
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
   
   while (GetTickCount() - irRxTime < IR_RX_QUIET_PERIOD) {
      // try to avoid ir remote interference
      Sleep(10);
   }
      
   if (!tiraTransmit(videoSource)) {
      updateState(videoSourceCmd->sz);
      Sleep(2500);   // give the switch time to complete the request
   }   
}         


static struct {
   BOOL threadActive=FALSE;
   BOOL exitThread=TRUE;
   BOOL flash;
   int mode;
} videoChange;       

void videoModeChangeThreadStart(BOOL flash, int mode) {
   HANDLE hThread;
   
   videoChange.exitThread = TRUE;
   
   while(videoChange.threadActive)
      Sleep(10);
   
   videoChange.threadActive=TRUE; videoChange.exitThread=FALSE;
   videoChange.flash=flash; videoChange.mode=mode;
   
   if (hThread=CreateThread(
         NULL,    // default security
         0,       // default stack size
         (LPTHREAD_START_ROUTINE) videoModeChangeThread,
         NULL,    // thread parameter
         0,       // run thread immediately
         NULL)    // ignore thread id
      ) {
      CloseHandle(hThread);
   } else {
      videoChange.threadActive = FALSE;
      printf("videoModeChangeThreadStart failed\n");
   }   
} 

// let user iterate through video modes with an ir remote.  The function
// that starts this thread will cause this thread to exit before starting
// another thread.  If this thread times out, it will queue up a message
// to change the video mode.  
// This function flashes a menu on the screen to acknowledge a user that 
// a video mode change request is pending
DWORD videoModeChangeThread(LPVOID param) {
   int i;
   char szModeCmd[128];
   
   if (videoChange.flash)
      msgQueueAdd(SZ_FEEDBACK_TV_MENU);   // display menu
      
   for (i=0; i<60 && !videoChange.exitThread; i++)
      Sleep(50);   // give user 3 seconds to iterate to next mode
      
   if (videoChange.flash)
      msgQueueAdd(SZ_FEEDBACK_TV_RETURN);   // erase menu
      
   if (!videoChange.exitThread) {
      struct rcCmd * rcCmd = cmdIdToRcCmd(videoChange.mode);
      strncat(strcpy(szModeCmd, "mode "), 
              rcCmd->sz, sizeof(szModeCmd));
      msgQueueAdd(szModeCmd);
   }
   
   videoChange.threadActive = FALSE;
   ExitThread(0);
}


// translate a numeric TCP/IP message to a series of zigbee commands that 
// will change the paired set top box channel
void sendZigbeeChannelRequest(const char * channel) {
   if (atoi(channel)) {
      int i;
      char digit[2];
      digit[1] = '\0';
      for (i=0; i<strlen(channel); i++) {
         digit[0] = channel[i];
         zrcCmdQueueAdd(szToCercCmd(digit));
      }
      zrcCmdQueueAdd(&cercCmd[ZRC_OK]);
   }   
}      
         
   
// the default sendItachCmd has parameters where subsequent sendItachCmds are
// INDEPENDENT and zero processing command delay
void sendItachCmd(int id, int mode) { sendItachCmd(id, mode, INDEPENDENT, 0); }

// set up parameters and call the function to process an itach command request
void sendItachCmd(int id, int mode, int dependent, int delay) {
   struct rcCmd * cmd = cmdIdToRcCmd(id);
  
   if (!cmd->irTx || !(mode==TV_MODE || mode==DVD_MODE))
      return;
      
   struct irTx * irTx = &cmd->irTx[mode];
   
   if (!irTx->code) {
      printf("sendItachCmd: error no %s codes for irTx %s\n", remoteModeToSz(mode), irTx->sz);
      return;
   }   
  
   if (!dependent)
      itachCmd->prerequisiteFailure=FALSE;
   
   if (debug)
      printf("sendItachCmd: %s\n", irTx->sz);
      
   if (itachSd.socket!=INVALID_SOCKET && irTx->code) {
      itachCmd->set(irTx, dependent, delay);
      processItachCmd();
   }   
}

// send an itach command request and report success or failure
// pace sending the itach cmd retryMax times or until success is confirmed
void processItachCmd() {
   const char * szCode = itachCmd->irTx->code;
   const char * szDesc = itachCmd->irTx->sz;
   struct irRx * irRx = itachCmd->irRx;
   int dependent=itachCmd->dependent;
   int delay=itachCmd->delay;
   HANDLE hThread;
   int retry, retryMax=2, replyWait;
   
   for (retry=0; retry<retryMax; retry++) {
      
      // pace to avoid ir remote interference
      while (GetTickCount() - irRxTime < 550 )
         Sleep(50);
         
      if (dependent && itachCmd->prerequisiteFailure) {
         // warn prerequisite processItachCmd failed to confirm
         printf("processItachCmd: warning prerequisite processItachCmd failed to confirm %s\n", szDesc);
         return;
      }
      
      if (retry) {
         printf("processItachCmd attempting retry %d irTx %s irRxPending: %d irComplete: %d\n\n", 
            retry, szDesc, itachCmd->irRxPending!=NULL, itachCmd->irComplete);
         Sleep(4000);  // let menu screens time out  
      } 
         
      if (itachSd.socket!=INVALID_SOCKET) {
         itachCmd->irRxPending=irRx;
         itachCmd->irComplete=FALSE;
         
         printf("processItachCmd requesting %s: %s\n", szTime(), szDesc); 
         sendMsg(&itachSd, szCode, szDesc);
      }   
         
      if (delay) {
         // allow itach commands time to process 
         // e.g. tv power on, video menu pop up, etc
         Sleep(delay);      
      }   
         
      for (replyWait=0; 
          !(!itachCmd->irRxPending && itachCmd->irComplete || replyWait>1500); 
          replyWait+=10) {
         // wait for the requested ir code AND a "completeir" tcp message from the
         // itach OR a replyWait timeout
         // Allow irRxPending to preempt a repyWait timeout but do not consider an
         // irRxPending timeout an error because plasma tv background noise makes 
         // confirmation unreliable.  Currently unable to locate the ir receiver
         // where it does not pick up ir noise from the plasma tv display   
         Sleep(10);
      }
      
      if (itachCmd->irComplete)  // && !itachCmd->irRxPending)
         break;
   }
   
   if (itachCmd->irComplete) { // && !itachCmd->irRxPending) {
      // a "completeir" tcp message was received from the itach
      printf("%s", itachCmd->debug);
      printf("processItachCmd success %s: %s\n\n", szTime(), szDesc); 
   }
   else {    
      printf("processItachCmd failed irTx %s %s irRxPending: %d irComplete: %d\n\n", 
             szDesc, szTime(), itachCmd->irRxPending!=NULL, itachCmd->irComplete);
      if (!dependent)
         itachCmd->prerequisiteFailure = TRUE;
   }   
   Sleep(IR_RX_QUIET_PERIOD);   // pause to minimize interference
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


BOOL isVideoPlayerActive() {
   int appId = getForegroundWindowAppID();
   return (appId==NETFLIX || appId==VLC ||
           appId==WMC || appId==YOUTUBE);
}           


// setNextForegroundWindow uses GetNextWindow and GetWindowText to 
// maintain a list of windows that partially match an entry in the 
// titles array.  After updating the list the next round-robin
// entry in the list is used to call SetForegroundWindow.  If no windows
// with partially matching titles exist, SetForegroundWindow is not called.
//
// Although the titles array can contain the partial name of any arbitrary 
// window title, the intended use is to iterate over explorer windows.  
// In order to differentiate explorer windows, it is important for the title 
// to fully qualify the current path.  In any explorer window, go to 
// Organize/Folder Options/View and check "Display the full path in the title 
// bar (Classic theme only)"
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
      unsigned count;  // # of windows partially matching an entry in the titles array
   } * wi;
   
   const char * titles[] = {
      "I:\\Recorded TV",
      "I:\\home\\jhm\\video\\lnk",
      "Windows Media Center",
      "Netflix - ",
      "YouTube - ",
      "youtube.com",
      "VLC media player",
      "Microsoft Silverlight"
   };
   
   if (!dwl)
      dwl = new LinkedList();  // desktop window list
   
   for (dwl=dwl->first(); dwl->isNotLast(); dwl=dwl->next())
      ((winInfo *)dwl->get())->count=0;
   
   // iterate to refresh the list of matching desktop windows.  If a titles[] 
   // entry partially matches a window title, add that HWND to the dwl list
   while(hWnd) {
      titleLen = GetWindowText(hWnd, title, sizeof(title));
      for (i=0; titleLen>0 && i<sizeof(titles)/sizeof(char *); i++) {
         if (strstr(title,titles[i]) && IsWindowVisible(hWnd)) {
            // titles[i] partially matches this HWND's title AND
            // this HWND has a visible window (a process may have one 
            // or more HWNDs with hidden windows)
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
      char * exe = (char *) malloc(strlen(systemRoot) + strlen(explorer) + 1);
      strcat(strcpy(exe, systemRoot), explorer); 
      createProcess(exe, path);
      free(exe);
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


// requested by audio receiver agent at startup
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


// requested by a client at startup
void sendStateInformation(QueuedMsg * msg) {
  sendMsg(msg->sd, tiraCmdIdToSz(audioPowerState));
  sendMsg(msg->sd, SZ_VIDEO_AUX);
  sendMsg(msg->sd, tiraCmdIdToSz(rearSpeakerState));
  sendMsg(msg->sd, tiraCmdIdToSz(frontSpeakerState));
  sendMsg(msg->sd, tiraCmdIdToSz(bSpeakerState));
  sendMsg(msg->sd, tiraCmdIdToSz(MASTER_VOLUME));
  sendMsg(msg->sd, tiraCmdIdToSz(CENTER_VOLUME));
  sendMsg(msg->sd, tiraCmdIdToSz(SURROUND_VOLUME));
  sendMsg(msg->sd, tiraCmdIdToSz(SUBWOOFER_VOLUME));
  sendMsg(msg->sd, tiraCmdIdToSz(surroundProfile));
  sendMsg(msg->sd, tiraCmdIdToSz(station));
  sendMsg(msg->sd, tiraCmdIdToSz(audioSource));
  sendMsg(msg->sd, tiraCmdIdToSz(muteState));
}   


// return the remote control command structure associated with the
// given tira ir code string
struct rcCmd * irRxCmdToRcCmd(char const * irCode) {

   int i,j,k;
   const char * tableCode;
   
   for (i=0; i<sizeof(rcCmd)/sizeof(struct rcCmd); i++) {
      for(j=0; rcCmd[i].irRx && j<rcCmd[i].irRx->size; j++) {
         tableCode = rcCmd[i].irRx->code[j];
         
         for (k=0;
              k<rcCmd[i].irRx->matchLen &&
              irCode[k]==tableCode[k]; k++);
              
         if (k==rcCmd[i].irRx->matchLen)
            return &rcCmd[i];
      }    
   }
   
   return &rcCmd[UNDEFINED];
}                


// return TRUE if given irRx has irCode in its array of possible codes
int irRxHasCode(struct irRx * irRx, char const * irCode) {

   int j,k;
   const char * tableCode;
   
   for(j=0; irRx && j<irRx->size; j++) {
      tableCode = irRx->code[j];
      
      for (k=0;
           k<irRx->matchLen &&
           irCode[k]==tableCode[k]; k++);
           
      if (k==irRx->matchLen)
         return TRUE;
   }    
   
   return FALSE;
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


// use the current foreground window as an index to return the shortcut keycode
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
   
   printf("Transmitting IR command %s: %s\n", szTime(), tiraCmdIdToSz(cmd));
   
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


// translate the current display mode to the corresponding remote mode enum 
// used as an index to provide a context aware itach wifi to ir command
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


const char * remoteModeToSz(int mode) {
   switch (mode) {
      case TV_MODE:
         return "tv mode";
      case DVD_MODE:
         return "dvd mode";
      default:
         return "undefined mode";
   }
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


