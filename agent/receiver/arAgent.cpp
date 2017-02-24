/* Abstract:
 * This module runs as a daemon process that will translate a TCP request
 * into an IR command via a USB attached Tira transmitter device.
 * After successfully processing a command request, this module
 * broadcasts a notification to update all TCP connected clients on the
 * network with the current status of the IR controlled device.
 *
 * History:
 * 2004 Aug 14 jhm original creation
 * 2006 Aug 05 jhm added den speaker and surround sound speaker 
 *                 control using xantech.com ir controlled cc-12 relays 
 *                 and sr-21 speaker selectors 
 * 2008 Jul 10 jhm added a powered subwoofer controlled by a xantech ac2 
 *                 ir switched ac outlet  
 * 2010 Dec 22 jhm eliminated using the ir switched ac outlet by utilizing
 *                 the receiver menu that directs the LFE channel to 
 *                 the LFE port for the SUBWOOFER_ON state or the main 
 *                 speakers for the SUBWOOFER_OFF state
 * 2011 Jun 29 jhm brought back the ir switched ac outlet to eliminate
 *                 static when the amplifier is off.  I can only surmise
 *                 the subwoofer input signal floats when the amplifier 
 *                 is off.
 * 2013 Apr 09 jhm improved tira dll loading to work from the current 
 *                 working directory 
 * 2013 Nov 24 jhm rewrote the DSP program state model to initialize
 *                 to a known configuration and update input sources
 *                 to a desired DSP program for specific speaker
 *                 configurations 
 * 2016 Jul 17 jhm added smart remote functionality
 *
 * Notes:
 * Use build.bat to build an executable
 * Use demo/demo.exe to generate the IR code char arrays to put into a 
 *    header file
 * Use client/msg/msg.exe to send a TCP test message to this server
 * Use client/receiver/arControl.java or client/surround/srControl.java
 *    as a GUI for sending TCP command messages to this server
 *
 * The arAgent target is built to run as a detached windows background 
 * process during startup or as a service.  The -mno-cygwin -mwindows 
 * options facilitate receiving the WM_ENDSESSION message by compiling and 
 * linking using minGW.  MinGW's philosophy differs from cygwin by using
 * native windows library support whereas Cygwin strives to provide complete
 * POSIX support.  The MinGW option comes with a Cygwin installation 
 * if all the development tools are installed.  The arAgentConsole target 
 * is used for debugging.
 *
 */  
 
#include <winsock2.h>
#include <stdio.h>
#include "../../common/tira.h"
#include "../../common/xantech_codes.h"
#include "../../common/yamaha_codes.h"
#include "../../common/common.h"
#include "../../common/extern.h"
#include "../../common/socket.h"
#include "arAgent.h"

int main(int argc, char ** argv) {

   int error, comPort;
   
   comPort=processArgs(argc, argv);
   
   loadTiraDll();
   printf("Tira library loaded\n");
   tira_init();
   
   int port = comPort ? comPort : DEFAULT_COM_PORT;
   printf("Tira activat%s on com port %d\n", 
          (error=tira_start(port-1)) ? "ion failed" : "ed", port);
   if (error) { usage(); exit(1); }
   
   HANDLE hThread = initExitHandler();
   initConsoleModeExitHandler();
   
   ampInit();
   
   initTcpServer(initWinsock(), MEDIA_CONTROL_PORT);
   
   isConsoleMode() ?
      ttyLoop() :
      WaitForSingleObject(hThread,INFINITE);
}

 
void ampInit() {

    // initialize the amplifier to a known state
    char szCmd[128];
  
    printf("Initializing amplifier\n");

    // set the flag to modify operations during initialization
    initializing = TRUE;

    // always start from a power off state
    tiraTransmit(AUDIO_POWER_OFF);
    
    // the matrix powers on to a state where all speakers are disconnected
    updateState(CENTER_SPEAKER_OFF);
    updateState(CHANNEL_A_TO_FRONT_SPEAKERS);
    updateState(REAR_SPEAKERS_FROM_CHANNEL_A);
    updateState(CHANNEL_B_OFF);
    updateState(CHANNEL_A_OFF);
    updateState(SUBWOOFER_OFF);
    
    // power on the amplifier and the matrix
    irSend(SZ_AUDIO_POWER_ON);     
    
    // set the amp to an unused input source to keep the center speaker quiet
    irSend(SZ_VIDEO_AUX);    
    
    // dial down to the volume floor
    //masterVolume=atoi(SZ_DEFAULT_MASTER_VOLUME);
    irSend(strcat(strcpy(szCmd, SZ_MASTER_VOLUME), "0"));
    
    // set the default radio station
    irSend(SZ_FM);
    tiraTransmit(FM1);

    // set the default DSP program for every input source   
    initDefaultInputSourceDspProgram();
    
    // select an unused source with dolby dts on before turning up volumes
    irSend(SZ_VIDEO_AUX);    
    
    irSend(strcat(strcpy(szCmd, SZ_CENTER_VOLUME), "0"));
    irSend(strcat(strcpy(szCmd, SZ_CENTER_VOLUME), 
                  SZ_DEFAULT_CENTER_VOLUME));
                
    irSend(strcat(strcpy(szCmd, SZ_SURROUND_RIGHT_VOLUME), "0"));
    irSend(strcat(strcpy(szCmd, SZ_SURROUND_RIGHT_VOLUME), 
                  SZ_DEFAULT_SURROUND_VOLUME));
                
    irSend(strcat(strcpy(szCmd, SZ_SURROUND_LEFT_VOLUME), "0"));
    irSend(strcat(strcpy(szCmd, SZ_SURROUND_LEFT_VOLUME), 
                  SZ_DEFAULT_SURROUND_VOLUME));
    
    irSend(SZ_SUBWOOFER_OFF);
    //setLFE(SUBWOOFER_ON);            
    irSend(strcat(strcpy(szCmd, SZ_SUBWOOFER_VOLUME), "0"));
    irSend(strcat(strcpy(szCmd, SZ_SUBWOOFER_VOLUME), 
                  SZ_DEFAULT_SUBWOOFER_VOLUME));
    //setLFE(SUBWOOFER_OFF);            
  
    irSend(strcat(strcpy(szCmd, SZ_MASTER_VOLUME), SZ_DEFAULT_MASTER_VOLUME));

    // clip quiet and loud sounds at low volumes to enhance voice quality
    // use "max" to maximize effect speaker output
    setSurroundProfile(SURROUND_STD);
    
    irSend(SZ_AUDIO_POWER_OFF);
    
    initializing = FALSE;
    
    printf("Initializing amplifier complete\n");
}


void irSend(const char * szCmd) {
             
    int i, cmd;
    static int restoring=FALSE;

    if ((cmd = tiraCmdSzToId(szCmd)) == UNDEFINED) {
        printf("irSend: Error command %s\n", szCmd);
        return;
    }

    if (!initializing)
       printf("irSend: %s\n", szCmd);
    
    char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
    char dspCmd = cmd>=DSP_MODE_CMD_RANGE_BEGIN && cmd<=DSP_MODE_CMD_RANGE_END;
    struct tiraCmd * irCmd = tiraCmdIdToTiraCmd(cmd);
    
    if (audioPowerState==AUDIO_POWER_OFF && 
        cmd!=AUDIO_POWER_ON &&
        cmd!=AUDIO_POWER_OFF &&
        cmd!=REGISTER &&
        cmd!=DEREGISTER &&
        cmd!=REREGISTER &&
        cmd!=EXIT) {
        printf("irSend: Ignoring %s request because amplifier power is off\n", szCmd);
        return;
    }
    
    switch (cmd) {
        case AUDIO_POWER_ON:

            if (!tiraTransmit(cmd)) {
                if (restoring)
                   break;

                if (initializing) {
                    // set the input to an unused source to insure a quiet setup
                    tiraTransmit(VIDEO_AUX);    
                }    
                
                broadcastMsg(SZ_ENABLE_CONTROLS);

                if (preserved) {
                    broadcastMsg(SZ_AUDIO_POWER_ON);
                    irSend(tiraCmdIdToSz(preserved->rearSpeakerState));
                    irSend(tiraCmdIdToSz(preserved->frontSpeakerState));
                    irSend(tiraCmdIdToSz(preserved->bSpeakerState));
                    irSend(tiraCmdIdToSz(preserved->audioSource));
                    preserved=NULL;
                }    
            }   
            break;
        case AUDIO_POWER_OFF:
            broadcastMsg(SZ_AUDIO_POWER_OFF);
            if (registrationCount) {
                preserved=&serverState;
                preserved->rearSpeakerState = rearSpeakerState;
                preserved->frontSpeakerState = frontSpeakerState;
                preserved->bSpeakerState = bSpeakerState;
                preserved->audioSource = audioSource;
            }    
            irSend(SZ_VIDEO_AUX);
            // set matrix to power off defaults
            updateState(CENTER_SPEAKER_OFF);
            updateState(CHANNEL_A_TO_FRONT_SPEAKERS);
            updateState(REAR_SPEAKERS_FROM_CHANNEL_A);    
            updateState(CHANNEL_B_OFF);
            updateState(CHANNEL_A_OFF);
            updateState(SUBWOOFER_OFF);
            //if (!registrationCount) {
            //    // restore volume settings
            //    char szCmd[128];
            //    if (masterVolume > atoi(SZ_DEFAULT_MASTER_VOLUME))
            //       irSend(strcat(strcpy(szCmd, SZ_MASTER_VOLUME), 
            //                     SZ_DEFAULT_MASTER_VOLUME));
            //    irSend(strcat(strcpy(szCmd, SZ_SURROUND_VOLUME), 
            //                  SZ_DEFAULT_SURROUND_VOLUME));
            //    irSend(strcat(strcpy(szCmd, SZ_CENTER_VOLUME), 
            //                  SZ_DEFAULT_CENTER_VOLUME));
            //    irSend(strcat(strcpy(szCmd, SZ_SUBWOOFER_VOLUME), 
            //                      SZ_DEFAULT_SUBWOOFER_VOLUME));
            //}    
            currentEffectSpeaker=0;
            tiraTransmit(AUDIO_POWER_OFF);
            break;    
        case SURROUND_MIN:   
        case SURROUND_STD:   
        case SURROUND_MAX:   
            setSurroundProfile(cmd);
            break;
        case REAR_SPEAKERS_ON:    
        case REAR_SPEAKERS_OFF:    
        case FRONT_SPEAKERS_ON:    
        case FRONT_SPEAKERS_OFF:
            setSwitchMatrix(cmd);
            break;
        case REGISTER:
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
            sendMsg(tiraCmdIdToSz(getDspEffectState()));
            sendMsg(tiraCmdIdToSz(audioPowerState));
        
            if (++registrationCount==1) {
               sendMsg(SZ_FIRST_REGISTRATION);
               irSend(SZ_AUDIO_POWER_ON);
            }   
            break;
        case DEREGISTER:    
            if (registrationCount)
                registrationCount--;
            if (!registrationCount) {
                if (audioPowerState==AUDIO_POWER_ON)
                    irSend(SZ_AUDIO_POWER_OFF);
                preserved = NULL;
            }    
            break;    
        case REREGISTER:
            if (++registrationCount==1) {
                sendMsg(SZ_RESTORE_STATE);
                restoring=TRUE;
            }    
            else {
                sendMsg(SZ_VIDEO_AUX);
                sendMsg(SZ_DSP_EFFECT_ON);
                sendMsg(tiraCmdIdToSz(rearSpeakerState));
                sendMsg(tiraCmdIdToSz(frontSpeakerState));
                sendMsg(tiraCmdIdToSz(bSpeakerState));
                sendMsg(tiraCmdIdToSz(MASTER_VOLUME));
                sendMsg(tiraCmdIdToSz(CENTER_VOLUME));
                sendMsg(tiraCmdIdToSz(SURROUND_VOLUME));
                sendMsg(tiraCmdIdToSz(SUBWOOFER_VOLUME));
                sendMsg(tiraCmdIdToSz(station));
                sendMsg(tiraCmdIdToSz(audioSource));
                sendMsg(tiraCmdIdToSz(muteState));
                sendMsg(tiraCmdIdToSz(getDspEffectState()));
                sendMsg(tiraCmdIdToSz(audioPowerState));
            }    
            break;
        case EXIT:
            shutdownServer();
            Sleep(2000);  // wait for client threads to deregister
            if (audioPowerState==AUDIO_POWER_ON) 
                tiraTransmit(AUDIO_POWER_OFF);
            printf("Exiting...\n");
            stty_buffered();
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
        case MUTE_ON:
        case MUTE_OFF:
            // mute always the last restore command    
            restoring=FALSE;
        default:
            if (irCmd && irCmd->state==&station && audioSource != FM && !restoring)
                tiraTransmit(FM);
        
            if (volumeCmd) {
                setVolume(szCmd);
            } else if (isValidCmdId(cmd)) {
                if (isCmdStateSet(cmd)) {
                    // updateState will broadcast state confirmation to the
                    // client requesting the command
                    updateState(cmd);
                } else {
                    tiraTransmit(cmd);
                }    
            } else {   
                printf("irSend: unknown command %d\n", cmd);
            }  
            
            if (irCmd) {
                if (irCmd->state==&audioSource && !initializing && !preserved) {
                    setInputSourceDspEffect();
                //} else if (irCmd->state==&subwooferState) { 
                //    setLFE(cmd);
                } 
            }        
    }
    
    if (cmd == REGISTER || cmd == DEREGISTER || cmd == REREGISTER) {
        printf("Processed a %s request. Count %d\n", tiraCmdIdToSz(cmd), registrationCount);
    }    
}         


void processAgentNotification(const char * server, char * msg) {}
void processClientCmd(const char * msg) { 
    printf("processClientCmd: %s\n", msg);
    irSend(msg); 
}
void processServerConnect(const char * agent) {}
void processClientDisconnect() { irSend(SZ_DEREGISTER); }


void setSwitchMatrix(int cmd) {
   int fss = frontSpeakerState, rss = rearSpeakerState;

    switch (cmd) {
        case REAR_SPEAKERS_ON:    
            if (frontSpeakerState==FRONT_SPEAKERS_OFF) {
                // stereo mode
                irSend(SZ_CHANNEL_A_TO_REAR_SPEAKERS);
                irSend(SZ_REAR_SPEAKERS_FROM_CHANNEL_A);
                irSend(SZ_CHANNEL_A_ON);
            } else {
                // surround sound mode
                irSend(SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL);
                irSend(SZ_SUBWOOFER_ON);
            }        
            break;    
        case REAR_SPEAKERS_OFF:    
            if (frontSpeakerState==FRONT_SPEAKERS_OFF) {
                // all speakers off
                irSend(SZ_CHANNEL_A_OFF);
            }    
            irSend(SZ_CHANNEL_A_TO_FRONT_SPEAKERS);
            irSend(SZ_REAR_SPEAKERS_FROM_CHANNEL_A);    
            irSend(SZ_SUBWOOFER_OFF);
            break;
        case FRONT_SPEAKERS_ON:    
            if (rearSpeakerState==REAR_SPEAKERS_ON) {
                // surround sound mode
                irSend(SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL);
                irSend(SZ_SUBWOOFER_ON);
            }    
            irSend(SZ_CHANNEL_A_TO_FRONT_SPEAKERS);
            irSend(SZ_CHANNEL_A_ON);
            irSend(SZ_CENTER_SPEAKER_ON);
            break;
        case FRONT_SPEAKERS_OFF:
            if (rearSpeakerState==REAR_SPEAKERS_OFF) {
                // all speakers off
                irSend(SZ_CHANNEL_A_OFF);
            } else {
                // stereo mode
                irSend(SZ_CHANNEL_A_TO_REAR_SPEAKERS);
                irSend(SZ_REAR_SPEAKERS_FROM_CHANNEL_A);
                irSend(SZ_CHANNEL_A_ON);
            }    
            irSend(SZ_SUBWOOFER_OFF);
            irSend(SZ_CENTER_SPEAKER_OFF);
            break;
    }

    if (fss!=frontSpeakerState)
       broadcastMsg(tiraCmdIdToSz(frontSpeakerState));
       
    if (rss!=rearSpeakerState)
       broadcastMsg(tiraCmdIdToSz(rearSpeakerState));
       
    setInputSourceDspEffect();
    
}    


void setSurroundProfileByVolume() {
    if (masterVolume>=MASTER_VOLUME_THRESHOLD_SURROUND_MAX &&
        surroundProfile!=SURROUND_MAX) {
        setSurroundProfile(SURROUND_MAX);
    } else if (masterVolume<MASTER_VOLUME_THRESHOLD_SURROUND_MAX &&
               masterVolume>MASTER_VOLUME_THRESHOLD_SURROUND_STD &&
               surroundProfile!=SURROUND_STD) {
        setSurroundProfile(SURROUND_STD);
    } else if (masterVolume<=MASTER_VOLUME_THRESHOLD_SURROUND_STD &&
               surroundProfile!=SURROUND_MIN) {
        setSurroundProfile(SURROUND_MIN);
    }
}        


void setLFE(int state) {

    int i,result;
    
    // yamaha menu 1d Speaker Set LFE/Bass Out
    // SWFR == send LFE signals to the subwoofer
    // MAIN == send LFE signals to the main speakers
    // BOTH == send LFE signals to both the main and the subwoofer
    
    if (state==subwooferState && !initializing)
        return;
    
    updateState(MUTE_OFF);
    
    printf("Setting LFE state to %s%s", 
           tiraCmdIdToSz(state),initializing ? "\n":"");
    
    tiraTransmit(MENU_SETTINGS);
    
    tiraTransmit(MENU_RIGHT);
    tiraTransmit(MENU_DOWN);
    tiraTransmit(MENU_DOWN);
    tiraTransmit(MENU_DOWN);
    tiraTransmit(MENU_RIGHT);
    tiraTransmit(MENU_RIGHT);
    
    if (state==SUBWOOFER_OFF)
        tiraTransmit(MENU_LEFT);
        
    for (i=result=0; i<5 && !result; i++)
        result=tiraTransmit(MENU_UP);
        
    if (!initializing)    
        printf("done\n");
    
} 


void setSurroundProfile(int cmd) {

    int i,result;
    static int currentProfile=UNDEFINED;
    
    // yamaha menu 10b dynamic range
    // surround min == maximizes voice frequencies
    // surround std == balances voice with surround
    // surround max == minimizes voice frequencies
    
    printf("Surround profile optimiz%s to %s%s", 
           currentProfile==cmd ? "ed" : "ing", 
           tiraCmdIdToSz(cmd), 
           initializing || currentProfile==cmd ? "\n":"");
           
    if (currentProfile==cmd)
        return;       
    
    tiraTransmit(MENU_SETTINGS);
    
    for (i=0; i<9; i++)
        tiraTransmit(MENU_DOWN);
        
    tiraTransmit(MENU_RIGHT);
    tiraTransmit(MENU_DOWN);
    tiraTransmit(MENU_DOWN);
    tiraTransmit(MENU_RIGHT);
    tiraTransmit(MENU_RIGHT);
    
    if (cmd==SURROUND_MAX)
        tiraTransmit(MENU_LEFT);
        
    if (cmd==SURROUND_MAX || cmd==SURROUND_STD)
        tiraTransmit(MENU_LEFT);
        
    for (i=result=0; i<13 && !result; i++)
        result=tiraTransmit(MENU_UP);
        
    if (!initializing)    
        printf("done\n");
    
    if (!result)    
        updateState(cmd);
} 


void initDefaultInputSourceDspProgram() {
   int i;
   printf("\n");    
   for (i=0; i<sizeof(dsp)/sizeof(struct dsp); i++) {
      tiraTransmit(dsp[i].audioSource);
      tiraTransmit(dsp[i].program);   // sets DSP_EFFECT_ON regardless of previous state
      if (dsp[i].state == DSP_EFFECT_OFF)
         tiraTransmit(DSP_EFFECT_OFF);
      printf("\n");    
   }
} 


int tiraTransmit(int cmd) {

   int result=0;
   char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
   char menuCmd = cmd>=MENU_RANGE_BEGIN && cmd<=MENU_RANGE_END;

   if (!volumeCmd && !menuCmd) {
      printf("Transmitting IR command: %s\n", tiraCmdIdToSz(cmd));
   }
   
   struct tiraCmd * tiraCmd = tiraCmdIdToTiraCmd(cmd);
   
   if (tiraCmd->codes) {
   
      if (!debug) {
         delay(cmd);
         result = tiraTransmit(tiraCmd);
         delay(cmd);
      }   
      
      if (!result && !initializing && (volumeCmd || menuCmd)) {
         printf(".");
         fflush(stdout);
      } 
       
   } else {
      printf("tiraTransmit: No IR code for IR command: %s\n", tiraCmdIdToSz(cmd));
      result = -1;
   }

   if (!result && !volumeCmd && !menuCmd)
      updateState(cmd);
       
   return result;
}


// this function does nothing if the given volCmd does
// not match one of the effect speaker volume commands
void activateSpeakerMenu(const char * volCmd) {
    // the yamaha amplifier powers on to the center volume menu
    // item but retains the most recently accessed effect speaker 
    // until powered off
    int i, requested;
    int * ces=&currentEffectSpeaker;
    static const char * effectSpeakerMenu[] = {SZ_CENTER_VOLUME,
                                               SZ_SURROUND_RIGHT_VOLUME,
                                               SZ_SURROUND_LEFT_VOLUME,
                                               SZ_SUBWOOFER_VOLUME};
                                         
    int menuSize = sizeof(effectSpeakerMenu)/sizeof(char *);
    
    for (i=0,requested=*ces;i<menuSize; i++) {
        if (strstr(volCmd,effectSpeakerMenu[i])) {
            requested=i;
    
            // activate the menu
            tiraTransmit(MENU_EFFECT_SPEAKER_LEVEL);
    
            // iterate to the requested menu
            while(*ces!=requested) {
                tiraTransmit(MENU_EFFECT_SPEAKER_LEVEL);
                *ces=(*ces+1) % menuSize;
            }
            
            break;      
        }    
    }
}


void updateState(int cmd) {
   char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
   char menuCmd = cmd>=MENU_RANGE_BEGIN && cmd<=MENU_RANGE_END;
   char matrixCmd = cmd>=MATRIX_CMD_RANGE_BEGIN && cmd<=MATRIX_CMD_RANGE_END;
   char dspCmd = cmd>=DSP_MODE_CMD_RANGE_BEGIN && cmd<=DSP_MODE_CMD_RANGE_END;
   
   struct tiraCmd * tiraCmd = tiraCmdIdToTiraCmd(cmd);
   
   switch (cmd) {
      case AUDIO_POWER_OFF:
      case AUDIO_POWER_ON:
      case GEORGIA:   
      case JIMSON:   
      case LVR_MM:
      case DEN_MM:
      case FM:
      case TV:   
      case DVD:
      case VIDEO_AUX:
         if (muteState==MUTE_ON && *tiraCmd->state!=cmd)
            updateState(MUTE_OFF);
   }         

   setState(tiraCmd->sz);
   
   if (dspCmd) {
      struct dsp * dsp = getDsp();
      if (dsp && !initializing) {
         if (cmd==DSP_EFFECT_ON || cmd==DSP_EFFECT_OFF || cmd==DSP_EFFECT_TOGGLE)
             dsp->state = (dsp->state==DSP_EFFECT_ON) ? DSP_EFFECT_OFF : DSP_EFFECT_ON;
      }       
   }   
   
    rearSpeakerState = channel_a_relay==CHANNEL_A_ON &&
                       channel_a_director==CHANNEL_A_TO_REAR_SPEAKERS &&
                       rear_speaker_selector==REAR_SPEAKERS_FROM_CHANNEL_A ||
                       rear_speaker_selector==REAR_SPEAKERS_FROM_REAR_CHANNEL ?
                          REAR_SPEAKERS_ON : REAR_SPEAKERS_OFF;
   frontSpeakerState = channel_a_relay==CHANNEL_A_ON &&
                       channel_a_director==CHANNEL_A_TO_FRONT_SPEAKERS ?
                          FRONT_SPEAKERS_ON : FRONT_SPEAKERS_OFF;

   if (!matrixCmd && !volumeCmd && !menuCmd && !dspCmd && !initializing && !preserved) {
      // MATRIX, VOLUME, MENU and DSP commands handle broadcast updates on their own.
      // Updates are not sent during initialization.  Updates are withheld and state 
      // information is preserved when a client powers off without deregistering.
      broadcastMsg(tiraCmdIdToSz(cmd));
   }
}


void setVolume(const char * volCmd) {

    int result;
    char stepSz[256], *volSz, surroundSpeaker=FALSE;
    static char recursive=FALSE;
    int cmdId=tiraCmdSzToId(volCmd);
    int stepId=cmdId;
    
    struct tiraCmd * irCmd = tiraCmdIdToTiraCmd(cmdId);
    if (cmdId<VOLUME_RANGE_BEGIN || cmdId>VOLUME_RANGE_END) {
        printf("setVolume: no irCmd for %s\n",volCmd);
        return;
    }        
    int newVolume, * currentVolume=irCmd->state;
    int maxVolume=strstr(volCmd,SZ_MASTER_VOLUME)==NULL ? 
        EFFECT_VOLUME_MAX : MASTER_VOLUME_MAX;
        
    strcpy(stepSz, volCmd);
    if (!(volSz=strstr(stepSz, SZ_VOLUME))) {
        printf("setVolume: no volume pattern in %s\n",volCmd);
        return;
    } 
    
    switch(cmdId) {
        case CENTER_VOLUME:
        case SURROUND_RIGHT_VOLUME:
        case SURROUND_LEFT_VOLUME:
        case SUBWOOFER_VOLUME:
            surroundSpeaker=TRUE;
        case MASTER_VOLUME:
            // these volume commands specify an absolute level
            newVolume = atoi(volSz + strlen(SZ_VOLUME));
            // truncate the stepSz absolute volume level integer 
            *(volSz + strlen(SZ_VOLUME)) = 0;
            // and then append the relative volume direction
            strcat(stepSz, newVolume>*currentVolume ? "up":"down"); 
            stepId=tiraCmdSzToId(stepSz);
            // set irCmd to the relative up or down command
            irCmd = tiraCmdIdToTiraCmd(stepId);
            break;
        case SURROUND_VOLUME:
            newVolume = atoi(volSz + strlen(SZ_VOLUME));
            updateState(MUTE_OFF); 
            recursive=TRUE;
            sprintf(stepSz, "%s%d", SZ_SURROUND_RIGHT_VOLUME, newVolume);
            setVolume(stepSz);
            sprintf(stepSz, "%s%d", SZ_SURROUND_LEFT_VOLUME, newVolume);
            setVolume(stepSz);
            recursive=FALSE;
            broadcastMsg(tiraCmdIdToSz(cmdId));
            return;
        case CENTER_VOLUME_UP:
        case SURROUND_RIGHT_VOLUME_UP:
        case SURROUND_LEFT_VOLUME_UP:
        case SUBWOOFER_VOLUME_UP:
            surroundSpeaker=TRUE;
        case MASTER_VOLUME_UP:
            newVolume=*currentVolume+3;
            break;
        case CENTER_VOLUME_DOWN:
        case SURROUND_RIGHT_VOLUME_DOWN:
        case SURROUND_LEFT_VOLUME_DOWN:
        case SUBWOOFER_VOLUME_DOWN:
            surroundSpeaker=TRUE;
        case MASTER_VOLUME_DOWN:
            newVolume=*currentVolume-3;
            break;
    }
    
    if (surroundSpeaker) {
        struct dsp * dsp = getDsp();
        if (getDspEffectState()==DSP_EFFECT_OFF || getDspEffectState()==UNDEFINED) {
            printf("setVolume: the command \"%s\" is invalid when audioSource %s is set to %s\n",
                   volCmd, tiraCmdIdToSz(audioSource), tiraCmdIdToSz(getDspEffectState()));
            return;       
        }
    }               
    
    if (*currentVolume!=newVolume) {     
        if (!recursive)
            updateState(MUTE_OFF); 
        
        // truncate stepSz to eliminate the "up" or "down" suffix
        *(volSz+strlen(SZ_VOLUME)-1) = 0;
        printf("Stepping the %s from %d to %d%s", 
            stepSz, *currentVolume, newVolume, initializing ? "\n":"");
       
        activateSpeakerMenu(volCmd);
    
        // convert each volume "unit" to issue two ir commands
        *currentVolume<<=1; newVolume<<=1; maxVolume<<=1;
        
        for (result=0; 
             *currentVolume>newVolume && *currentVolume>=0 && !result; 
             *currentVolume=*currentVolume-1) {
            result=tiraTransmit(stepId);
        }   
    
        for (result=0; 
             *currentVolume<newVolume && *currentVolume<maxVolume && !result; 
             *currentVolume=*currentVolume+1) {
            result=tiraTransmit(stepId);
        }    
    
        // convert back to one volume "unit" per two ir commands
        *currentVolume>>=1;
       
        // Xantech devices require a pause before and after a command.  The
        // following delay insures command distinction.  The Yamaha amplifier
        // amplifier has no problem with back-to-back commands.
        if (stepId!=MASTER_VOLUME_UP && stepId!=MASTER_VOLUME_DOWN && !debug) {
            // a delay is needed to time out of the effect speaker level menu
            Sleep(3000);
        }    
            
        if (!initializing) {
            printf("done\n");
            if (!recursive)
                broadcastMsg(tiraCmdIdToSz(stepId));
            //setSurroundProfileByVolume();
        }   
    }       
}


void delay(int cmd) {
    
   if (cmd>=DSP_MODE_CMD_RANGE_BEGIN && cmd<=DSP_MODE_CMD_RANGE_END)
       Sleep(DSP_CMD_DELAY);
   
   if (cmd>=XANTECH_CMD_RANGE_BEGIN && cmd<=XANTECH_CMD_RANGE_END)
       Sleep(XANTECH_DELAY);    // required for command distinction
       
   if (cmd==AUDIO_POWER_ON && audioPowerState==AUDIO_POWER_OFF ||  // allow amplifier time to power up before sending commands
       cmd==AUDIO_POWER_OFF && audioPowerState==AUDIO_POWER_ON) {  // and finish processing commands before powering down
       Sleep(AUDIO_POWER_ON_OFF_DELAY);   
   }    
}           


BOOL isCmdStateSet(int cmd) {
   char dspCmd = cmd>=DSP_MODE_CMD_RANGE_BEGIN && cmd<=DSP_MODE_CMD_RANGE_END;
   return dspCmd ? getDspEffectState()==cmd : isTiraCmdStateSet(cmd);
}


struct dsp * getDsp() {
   int i;
   for (i=0; i<sizeof(dsp)/sizeof(struct dsp); i++) {
      if (dsp[i].audioSource==audioSource)
         return &dsp[i];
   }
   return NULL;
}


int getDspEffectState() {
   struct dsp * dsp = getDsp();
   return (dsp ? dsp->state : UNDEFINED);
}    


int isDspEffectToggleEnabled() {
    struct dsp * dsp = getDsp();
    return (dsp ? dsp->toggleEnable==DSP_EFFECT_TOGGLE_ENABLE : FALSE);
}


// The DSP_EFFECT_TOGGLE is used instead of the DSP_DOLBY_NORMAL command
// because if DSP_DOLBY_NORMAL happens to be sent twice within three seconds it
// will toggle between the "Normal" and "Enhanced" sub-programs
void setInputSourceDspEffect() {
    static int previousState=UNDEFINED;
    
    if (isDspEffectToggleEnabled()) {
        if ((frontSpeakerState==FRONT_SPEAKERS_ON && 
             rearSpeakerState==REAR_SPEAKERS_ON &&
             getDspEffectState()==DSP_EFFECT_OFF) ||
             (frontSpeakerState==FRONT_SPEAKERS_OFF ||
             rearSpeakerState==REAR_SPEAKERS_OFF) &&
             getDspEffectState()==DSP_EFFECT_ON) {
             // DSP only desirable for surround sound 
             irSend(SZ_DSP_EFFECT_TOGGLE);
        }   
    }
    
    if (previousState != getDspEffectState()) {
       broadcastMsg(tiraCmdIdToSz(getDspEffectState()));
       previousState = getDspEffectState();
    }   
}


void printIrCmd(struct irCmd * irCmd) {
    int i;
    printf("id: %d\n", irCmd->id);
    printf("sz: %s\n", irCmd->sz);
    printf("state: %d\n", *(irCmd->state));
    printf("size: %d\n", irCmd->size);
    printf("codes:\n");
    for (i=0; i<irCmd->size; i++) {
        printf("0x%02X, ", irCmd->codes[i]);
        if (((i+1) % 10) == 0) printf("\n");
    }
    printf("\n");
}    


void help() {
   printf( "\ntype \'q\' to quit\n");
}

