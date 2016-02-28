/* Abstract:
 * This module runs as a daemon process that will translate a UDP request
 * into an IR command via a USB attached Tira transmitter device.
 * After successfully processing a command request, this module
 * broadcasts a UDP notification to update all clients on the network
 * with the current status of the IR controlled device.
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
 *
 * Notes:
 * Use build.bat to build an executable
 * Use demo/demo.exe to generate the IR code char arrays to put into a 
 *    header file
 * Use client/msg/msg.exe to send UDP test messages to this server
 * Use client/amplifer/acClient.java or client/surround/scClient.java
 *    as a GUI for sending UDP command messages to this server
 *
 * The acServer target is built to run as a detached windows background 
 * process during startup or as a service.  The -mno-cygwin -mwindows 
 * options facilitate receiving the WM_ENDSESSION message by compiling and 
 * linking using minGW.  MinGW's philosophy differs from cygwin by using
 * native windows library support whereas Cygwin strives to provide complete
 * POSIX support.  The MinGW option comes with a Cygwin installation 
 * if all the development tools are installed.  The acServerConsole target 
 * is used for debugging.
 *
 */  
 
#include <winsock2.h>
#include <stdio.h>
#include "../common/tira.h"
#include "../common/xantech_codes.h"
#include "../common/yamaha_codes.h"
#include "../common/common.h"
#include "../common/extern.h"
#include "acServer.h"

int main(int argc, char ** argv) {

   int error, comPort;
   
   comPort=processArgs(argc, argv);
   
   Data = (const unsigned char *) GlobalAlloc(GPTR, 1024);
   initSockets();
   loadTiraDll();
   printf("Tira library loaded\n");
   tira_init();
   
   int port = comPort ? comPort : DEFAULT_COM_PORT;
   
   printf("Tira activat%s on com port %d\n", 
          (error=tira_start(port-1)) ? "ion failed" : "ed", port);
   if (error) { printf(usage); exit(1); }
   
   initConsoleModeExitHandler();
   initWindowsModeExitHandler();
   
   ampInit();
   
   while (1) {
       irSend(getClientMsg());
   }    
}

 
void ampInit() {

    // initialize the amplifier to a known state
    char szCmd[128];
  
    printf("Initializing amplifier\n");

    // set the flag to modify operations during initialization
    initializing = 1;

    // always start from a power off state
    irTransmit(POWER_OFF);
    
    // the matrix powers on to a state where all speakers are disconnected
    setState(CHANNEL_A_TO_FRONT_SPEAKERS);
    setState(REAR_SPEAKERS_FROM_AB_SWITCH);
    setState(CHANNEL_A_RELAY_OFF);
    setState(CHANNEL_B_SPEAKERS_OFF);
    setState(SUBWOOFER_OFF);
    
    // power on the amplifier and the matrix
    irSend(SZ_POWER_ON);     
    
    // set the amp to an unused input source to keep the center speaker quiet
    irSend(SZ_VIDEO_AUX);    
    
    // dial down to the volume floor
    //masterVolume=atoi(SZ_DEFAULT_MASTER_VOLUME);
    irSend(strcat(strcpy(szCmd, SZ_MASTER_VOLUME), "0"));
    
    // set the default radio station
    irSend(SZ_FM);
    irTransmit(FM1);

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
    
    irSend(SZ_POWER_OFF);
    
    initializing = 0;
    
    printf("Initializing amplifier complete\n");
}


void irSend(char * szCmd) {
             
    int i, cmd;
    if ((cmd = cmdSzToId(szCmd)) == UNDEFINED) {
        printf("irSend: Error command %s\n", szCmd);
        return;
    }    
    char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
    char dspCmd = cmd>=DSP_MODE_CMD_RANGE_BEGIN && cmd<=DSP_MODE_CMD_RANGE_END;
    struct irCmd * irCmd = cmdIdToIrCmd(cmd);
    
    if (powerState==POWER_OFF && 
        cmd!=POWER_ON &&
        cmd!=POWER_OFF &&
        cmd!=REGISTER &&
        cmd!=DEREGISTER &&
        cmd!=EXIT) {
        printf("irSend: Ignoring %s request because amplifier power is off\n", szCmd);
        return;
    }
    
    switch (cmd) {
        case POWER_ON:
            if (!irTransmit(cmd)) {
                if (initializing) {
                    // set the input to an unused source to insure a quiet 
                    // center speaker
                    irTransmit(VIDEO_AUX);    
                }    
                broadcastMsg(SZ_ENABLE_CONTROLS);
                
                if (preserved) {
                    broadcastMsg(SZ_POWER_ON);
                    irSend(cmdIdToSz(preserved->rearSpeakerState));
                    irSend(cmdIdToSz(preserved->frontSpeakerState));
                    irSend(cmdIdToSz(preserved->denSpeakerState));
                    irSend(cmdIdToSz(preserved->inputSource));
                    preserved=NULL;
                }    
                else if (registrationCount) {  // assume initialization forthcoming
                    irSend(cmdIdToSz(frontSpeakerState));
                    irSend(cmdIdToSz(rearSpeakerState));
                }           
            }    
            break;
        case POWER_OFF:
            broadcastMsg(SZ_POWER_OFF);
            if (registrationCount) {
                preserved=&serverState;
                preserved->rearSpeakerState = rearSpeakerState;
                preserved->frontSpeakerState = frontSpeakerState;
                preserved->denSpeakerState = denSpeakerState;
                preserved->inputSource = inputSource;
            }    
            irSend(SZ_VIDEO_AUX);    
            irSend(SZ_CHANNEL_A_RELAY_OFF);
            irSend(SZ_CHANNEL_A_TO_FRONT_SPEAKERS);
            irSend(SZ_REAR_SPEAKERS_FROM_AB_SWITCH);    
            irSend(SZ_CHANNEL_B_SPEAKERS_OFF);
            irSend(SZ_SUBWOOFER_OFF);
            if (!registrationCount) {
                char szCmd[128];
                if (masterVolume > atoi(SZ_DEFAULT_MASTER_VOLUME))
                    irSend(strcat(strcpy(szCmd, SZ_MASTER_VOLUME), 
                                  SZ_DEFAULT_MASTER_VOLUME));
                irSend(strcat(strcpy(szCmd, SZ_SURROUND_VOLUME), 
                              SZ_DEFAULT_SURROUND_VOLUME));
                irSend(strcat(strcpy(szCmd, SZ_CENTER_VOLUME), 
                              SZ_DEFAULT_CENTER_VOLUME));
                irSend(strcat(strcpy(szCmd, SZ_SUBWOOFER_VOLUME), 
                                  SZ_DEFAULT_SUBWOOFER_VOLUME));
            }    
            currentEffectSpeaker=0;
            irTransmit(POWER_OFF);
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
            broadcastMsg(cmdIdToSz(MASTER_VOLUME));
            broadcastMsg(cmdIdToSz(CENTER_VOLUME));
            broadcastMsg(cmdIdToSz(SURROUND_VOLUME));
            broadcastMsg(cmdIdToSz(SUBWOOFER_VOLUME));
            broadcastMsg(cmdIdToSz(denSpeakerState));
            broadcastMsg(cmdIdToSz(station));
            broadcastMsg(cmdIdToSz(surroundProfile));
            if (++registrationCount==1) {
                broadcastMsg(SZ_FIRST_REGISTRATION);
                irTransmit(POWER_ON);
            } else {
                broadcastMsg(cmdIdToSz(getDspState()));
                broadcastMsg(cmdIdToSz(powerState));
                broadcastMsg(cmdIdToSz(inputSource));
                broadcastMsg(cmdIdToSz(rearSpeakerState));
                broadcastMsg(cmdIdToSz(frontSpeakerState));
                broadcastMsg(cmdIdToSz(subwooferState));
                broadcastMsg(cmdIdToSz(muteState));
                if (powerState==POWER_ON)
                    broadcastMsg(SZ_ENABLE_CONTROLS);
            }    
            break;
        case DEREGISTER:    
            if (registrationCount)
                registrationCount--;
            if (!registrationCount) {
                if (powerState==POWER_ON)
                    irSend(SZ_POWER_OFF);
                preserved = NULL;
            }    
            break;    
        case EXIT:
            if (powerState==POWER_ON) 
                irSend(SZ_POWER_OFF);
            unloadTiraDll();
            FreeLibrary(handle);
            closesocket(rxSocket);
            closesocket(txSocket);
            WSACleanup();
            printf("Exiting...\n");
            exit(0);
        default:
            if (irCmd && irCmd->state==&station && inputSource != FM) {
                    irTransmit(FM);
            }    
        
            if (volumeCmd) {
                setVolume(szCmd);
            } else if (isValidCmdId(cmd)) {
                if (isCmdStateSet(cmd)) {
                    // setState will broadcast state confirmation to the
                    // client requesting the command
                    setState(cmd);
                } else {
                    irTransmit(cmd);
                }    
            } else {   
                printf("irSend: unknown command %d\n", cmd);
            }  
            
            if (irCmd) {
                if (irCmd->state==&inputSource && !initializing) {
                    setInputSourceDSP();
                //} else if (irCmd->state==&subwooferState) { 
                //    setLFE(cmd);
                } 
            }        
    }
    
    if (cmd == REGISTER || cmd == DEREGISTER) {
        printf("Processed a %s request. Count %d\n", cmdIdToSz(cmd), registrationCount);
    }    
}         


char * getClientMsg() {
    //
    // Wait for data from the client
    //
    int nLen = sizeof(SOCKADDR);
    int nRet;
    static char msgBuf[256];
    char client[128] = "?";

    memset(msgBuf, 0, sizeof(msgBuf));
    nRet = recvfrom(rxSocket,          // Bound socket
                    msgBuf,            // Receive buffer
                    sizeof(msgBuf),    // Size of buffer in bytes
                    0,                 // Flags
                    &saClient,         // Buffer to receive client address 
                    &nLen);            // Length of client address buffer
                    
    if (nRet == SOCKET_ERROR)
    {
        printWinsockError("recvfrom");
        closesocket(rxSocket);
        exit(1);
    }

    //getnameinfo(saClient,nLen,client,sizeof(client),NULL,0,0);

    //
    // Show that we've received some data
    //
    printf("\nData received: %s\n", msgBuf);
    //printf("Data received from %s: %s\n", client, msgBuf);
    return msgBuf;
}    


void setSwitchMatrix(int cmd) {
    switch (cmd) {
        case REAR_SPEAKERS_ON:    
            if (frontSpeakerState==FRONT_SPEAKERS_OFF) {
                // stereo mode
                irSend(SZ_CHANNEL_A_TO_AB_SWITCH);
                irSend(SZ_REAR_SPEAKERS_FROM_AB_SWITCH);
                irSend(SZ_CHANNEL_A_RELAY_ON);
            } else {
                // surround sound mode
                irSend(SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL);
                irSend(SZ_SUBWOOFER_ON);
            }        
            broadcastMsg(cmdIdToSz(rearSpeakerState));
            break;    
        case REAR_SPEAKERS_OFF:    
            if (frontSpeakerState==FRONT_SPEAKERS_OFF) {
                // all speakers off
                irSend(SZ_CHANNEL_A_RELAY_OFF);
            }    
            irSend(SZ_CHANNEL_A_TO_FRONT_SPEAKERS);
            irSend(SZ_REAR_SPEAKERS_FROM_AB_SWITCH);    
            irSend(SZ_SUBWOOFER_OFF);
            broadcastMsg(cmdIdToSz(rearSpeakerState));
            break;
        case FRONT_SPEAKERS_ON:    
            if (rearSpeakerState==REAR_SPEAKERS_ON) {
                // surround sound mode
                irSend(SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL);
                irSend(SZ_SUBWOOFER_ON);
            }    
            irSend(SZ_CHANNEL_A_TO_FRONT_SPEAKERS);
            irSend(SZ_CHANNEL_A_RELAY_ON);
            broadcastMsg(cmdIdToSz(frontSpeakerState));
            break;
        case FRONT_SPEAKERS_OFF:
            if (rearSpeakerState==REAR_SPEAKERS_OFF) {
                // all speakers off
                irSend(SZ_CHANNEL_A_RELAY_OFF);
            } else {
                // stereo mode
                irSend(SZ_CHANNEL_A_TO_AB_SWITCH);
                irSend(SZ_REAR_SPEAKERS_FROM_AB_SWITCH);
                irSend(SZ_CHANNEL_A_RELAY_ON);
            }    
            irSend(SZ_SUBWOOFER_OFF);
            broadcastMsg(cmdIdToSz(frontSpeakerState));
            break;
    }
    
    setInputSourceDSP();
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
    
    setState(MUTE_OFF);
    
    printf("Setting LFE state to %s%s", 
           cmdIdToSz(state),initializing ? "\n":"");
    
    irTransmit(MENU_SETTINGS);
    
    irTransmit(MENU_RIGHT);
    irTransmit(MENU_DOWN);
    irTransmit(MENU_DOWN);
    irTransmit(MENU_DOWN);
    irTransmit(MENU_RIGHT);
    irTransmit(MENU_RIGHT);
    
    if (state==SUBWOOFER_OFF)
        irTransmit(MENU_LEFT);
        
    for (i=result=0; i<5 && !result; i++)
        result=irTransmit(MENU_UP);
        
    if (!initializing)    
        printf("done\n");
    
} 


void setSurroundProfile(int cmd) {

    int i,result;
    
    // yamaha menu 10b dynamic range
    // surround min == maximizes voice frequencies
    // surround std == balances voice with surround
    // surround max == minimizes voice frequencies
    
    printf("Optimizing surround profile to %s%s", 
           cmdIdToSz(cmd),initializing ? "\n":"");
    
    irTransmit(MENU_SETTINGS);
    
    for (i=0; i<9; i++)
        irTransmit(MENU_DOWN);
        
    irTransmit(MENU_RIGHT);
    irTransmit(MENU_DOWN);
    irTransmit(MENU_DOWN);
    irTransmit(MENU_RIGHT);
    irTransmit(MENU_RIGHT);
    
    if (cmd==SURROUND_MAX)
        irTransmit(MENU_LEFT);
        
    if (cmd==SURROUND_MAX || cmd==SURROUND_STD)
        irTransmit(MENU_LEFT);
        
    for (i=result=0; i<13 && !result; i++)
        result=irTransmit(MENU_UP);
        
    if (!initializing)    
        printf("done\n");
    
    if (!result)    
        setState(cmd);
} 


// this function does nothing if the given volCmd does
// not match one of the effect speaker volume commands
void activateSpeakerMenu(char * volCmd) {
    // the yamaha amplifier powers on to the center volume menu
    // item but retains the most recently accessed effect speaker 
    // until powered off
    int i, requested;
    int * ces=&currentEffectSpeaker;
    static char * effectSpeakerMenu[] = {SZ_CENTER_VOLUME,
                                         SZ_SURROUND_RIGHT_VOLUME,
                                         SZ_SURROUND_LEFT_VOLUME,
                                         SZ_SUBWOOFER_VOLUME};
                                         
    int menuSize = sizeof(effectSpeakerMenu)/sizeof(char *);
    
    for (i=0,requested=*ces;i<menuSize; i++) {
        if (strstr(volCmd,effectSpeakerMenu[i])) {
            requested=i;
    
            // printf("%slevel requested\n", effectSpeakerMenu[requested]); 
    
            // activate the menu
            irTransmit(MENU_EFFECT_SPEAKER_LEVEL);
            // printf("%slevel activated\n", effectSpeakerMenu[*ces]); 
    
            // iterate to the requested menu
            while(*ces!=requested) {
                irTransmit(MENU_EFFECT_SPEAKER_LEVEL);
                *ces=(*ces+1) % menuSize;
                // printf("%slevel activated\n", effectSpeakerMenu[*ces]); 
            }
            
            break;      
        }    
    }
}        
        

void setVolume(char * volCmd) {

    int result;
    char stepSz[256], *volSz, surroundSpeaker=FALSE;
    static char recursive=FALSE;
    int cmdId=cmdSzToId(volCmd);
    int stepId=cmdId;
    
    struct irCmd * irCmd = cmdIdToIrCmd(cmdId);
    if (!irCmd) {
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
            stepId=cmdSzToId(stepSz);
            // set irCmd to the relative up or down command
            irCmd = cmdIdToIrCmd(stepId);
            break;
        case SURROUND_VOLUME:
            newVolume = atoi(volSz + strlen(SZ_VOLUME));
            setState(MUTE_OFF); 
            recursive=TRUE;
            sprintf(stepSz, "%s%d", SZ_SURROUND_RIGHT_VOLUME, newVolume);
            setVolume(stepSz);
            sprintf(stepSz, "%s%d", SZ_SURROUND_LEFT_VOLUME, newVolume);
            setVolume(stepSz);
            recursive=FALSE;
            broadcastMsg(cmdIdToSz(cmdId));
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
        if (getDspState()==DSP_EFFECT_OFF || getDspState()==UNDEFINED) {
            printf("setVolume: the command \"%s\" is invalid when inputSource %s is set to %s\n",
                   volCmd, cmdIdToSz(inputSource), cmdIdToSz(getDspState()));
            return;       
        }
    }               
    
    if (*currentVolume!=newVolume) {     
        if (!recursive)
            setState(MUTE_OFF); 
        
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
            result=irTransmit(stepId);
        }   
    
        for (result=0; 
             *currentVolume<newVolume && *currentVolume<maxVolume && !result; 
             *currentVolume=*currentVolume+1) {
            result=irTransmit(stepId);
        }    
    
        // convert back to one volume "unit" per two ir commands
        *currentVolume>>=1;
       
        // Xantech devices require a pause before and after a command.  The
        // following delay insures command distinction.  The Yamaha amplifier
        // amplifier has no problem with back-to-back commands.
        if (stepId!=MASTER_VOLUME_UP && stepId!=MASTER_VOLUME_DOWN) {
            // a delay is needed to time out of the effect speaker level menu
            Sleep(3000);
        }    
            
        if (!initializing) {
            printf("done\n");
            if (!recursive)
                broadcastMsg(cmdIdToSz(stepId));
            //setSurroundProfileByVolume();
        }   
    }       
}    


int getDspMode() {
    return (getDspState()==DSP_EFFECT_ON ? getDspProgram() : getDspState());
}    


int getDspProgram() {
    struct dsp * dsp = getDsp();
    return (dsp ? dsp->program : UNDEFINED);
}    


int getDspToggle() {
    struct dsp * dsp = getDsp();
    return (dsp ? dsp->toggle : UNDEFINED);
}    


void setInputSourceDSP() {
    if (getDspToggle()==DSP_EFFECT_TOGGLE_ENABLE) {
        if ((frontSpeakerState==FRONT_SPEAKERS_ON && 
             rearSpeakerState==REAR_SPEAKERS_ON &&
             getDspState()==DSP_EFFECT_OFF) ||
             (frontSpeakerState==FRONT_SPEAKERS_OFF ||
             rearSpeakerState==REAR_SPEAKERS_OFF) &&
             getDspState()==DSP_EFFECT_ON) {
             // DSP only desirable for surround sound 
             irSend(SZ_DSP_EFFECT_TOGGLE);
             broadcastMsg(cmdIdToSz(getDspMode()));
         }   
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


int processArgs(int argc, char ** argv) {
   int i, startupDelay, comPort=0;

   for (i=0;i<argc;i++) {
      if(!strcmp(argv[i],"-port") || !strcmp(argv[i],"-delay"));
      else if (i && !strcmp(argv[i-1],"-port"))
         comPort = atoi(argv[i]);
      else if (i && !strcmp(argv[i-1],"-delay")) {
         // delay startup during boot to allow dependencies to initialize
         if ( (startupDelay=atoi(argv[i])) && ((GetTickCount() < 60000) || isConsoleMode()) ) {
            printf("%d second startup delay...", startupDelay);
            fflush(stdout);
            Sleep(startupDelay * 1000);
            printf("done\n");
         }   
      } else if (i) {
         printf(usage);
         exit(0);
      }   
   }
   return comPort;
}   


// check if the application filename ends with "Console.exe"
BOOL isConsoleMode() {
    STARTUPINFO si;
    char * consoleSuffix = "Console.exe";
    GetStartupInfo(&si);
    return !(strcmp(&si.lpTitle[strlen(si.lpTitle)-strlen(consoleSuffix)],
                    consoleSuffix));
}      


void dispatchThread() {
    MSG msg;
    
    // create an invisible window to facilitate messages for the exitHandler
	HWND dialog = CreateWindowEx(
	   0,WC_DIALOG,"acServer Dispatch Thread",WS_OVERLAPPEDWINDOW,
	   400,100,200,200,NULL,NULL,NULL,NULL
	);
    
	SetWindowLong(dialog, GWL_WNDPROC, (long)windowsModeExitHandler);
	
	while(GetMessage(&msg,NULL,0,0))  {
	   TranslateMessage(&msg);
	   DispatchMessage(&msg); 
	} 
}


HANDLE initWindowsModeExitHandler() {
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


LRESULT WINAPI windowsModeExitHandler(HWND hwnd, 
                                      UINT msg, 
                                      WPARAM wParam, 
                                      LPARAM lParam) {
    switch(msg) {
       case WM_ENDSESSION:
       case WM_CLOSE:
          irSend(SZ_EXIT);
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


BOOL consoleModeExitHandler( DWORD event ) { 
  switch( event ) 
  { 
    case CTRL_C_EVENT: 
       printf("received a ctrl-c signal\n");
    case CTRL_CLOSE_EVENT: 
    case CTRL_SHUTDOWN_EVENT: 
       irSend(SZ_EXIT);
       return TRUE;
 
    // Pass other signals to the next handler. 
    case CTRL_BREAK_EVENT: 
    case CTRL_LOGOFF_EVENT: 
       return FALSE; 
 
    default: 
       return FALSE; 
  } 
} 


void initSockets() {

    WORD wVersionRequested = MAKEWORD(1,1);
    WSADATA wsaData;
    int nRet;

    //
    // Initialize WinSock and check version
    //
    nRet = WSAStartup(wVersionRequested, &wsaData);
    if (wsaData.wVersion != wVersionRequested) {    
        printf("\ninitSockets: wrong winsock version\n");
        exit(1);
    }
    
    //
    // Create a UDP/IP datagram socket
    //
    rxSocket = socket(AF_INET,        // Address family
                       SOCK_DGRAM,    // Socket type
                       IPPROTO_UDP);  // Protocol
    if (rxSocket==INVALID_SOCKET) {
        printWinsockError("initSockets: rx create socket");
        exit(1);
    }
    
    //
    // Fill in the address structure
    //
    SOCKADDR_IN saServer;

    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY; // Let WinSock assign address
    saServer.sin_port = htons(AUDIO_CONTROL_REQUEST_PORT);     

    //
    // bind the name to the socket
    //
    nRet = bind(rxSocket,                   // Socket descriptor
                (LPSOCKADDR)&saServer,      // Address to bind to
                sizeof(struct sockaddr)     // Size of address
                );
    if (nRet==SOCKET_ERROR) {
        printWinsockError("initSockets bind");
        closesocket(rxSocket);
        exit(1);
    }
    
    // Create a UDP/IP transmit datagram socket
    txSocket= socket(AF_INET,         // Address family
                     SOCK_DGRAM,      // Socket type
                     IPPROTO_UDP);    // Protocol
    if (txSocket==INVALID_SOCKET) {
        printWinsockError("initSockets: tx create socket");
        exit(1);
    }
    
    BOOL bOptVal = TRUE;
    int bOptLen = sizeof(BOOL);

    if (setsockopt(txSocket, 
                   SOL_SOCKET, 
                   SO_BROADCAST, 
                   (char*)&bOptVal, 
                   bOptLen) == SOCKET_ERROR) {
        printf("initSockets: Unable to set SO_BROADCAST on");
    }
}







