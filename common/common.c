#include <winsock2.h>
#include <stdio.h>
#include <unistd.h>
#include "../common/tira.h"
#include "../common/xantech_codes.h"
#include "../common/yamaha_codes.h"
#include "../common/common.h"
#include "../common/xantech_codes.c"
#include "../common/yamaha_codes.c"

const unsigned char * Data;
HMODULE handle = 0;
int initializing;
SOCKET txSocket;

// initializing to the maximum volume levels insures sufficient "down"
// volume commands will be issued during ampInit to reach volume floors
int masterVolume=MASTER_VOLUME_MAX;
int centerVolume=EFFECT_VOLUME_MAX;
int subwooferVolume=EFFECT_VOLUME_MAX;
int surroundRightVolume=EFFECT_VOLUME_MAX;
int surroundLeftVolume=EFFECT_VOLUME_MAX;
int inputSource=VIDEO_AUX;
int station=FM1;
int currentEffectSpeaker=0;
int powerState, muteState, poweringOff;
int registrationCount, surroundProfile;
int channelA_Relay, channelA_Switch, rearSpeakerSwitch;
int rearSpeakerState, frontSpeakerState, denSpeakerState;
int subwooferState, LFE_State;
int undefined = UNDEFINED;

struct serverState {
    int rearSpeakerState; 
    int frontSpeakerState; 
    int denSpeakerState;
    int inputSource;
} serverState, *preserved=NULL;        


// The default mode for stereo sources is DSP_EFFECT_OFF which means
// the center, rear and subwoofer speakers are disabled.
// DSP_EFFECT_ON is used to upmix stereo and decode surround sound
// using the desired program
//
// HD sources are all hardcoded to use Dolby Normal decoding with DSP_EFFECT_ON.
// The Dolby Enhanced sub-mode does not seemt to utilize the center channel 
// very well whenever the audio source is stereo.
//
// DSP is toggled on and off by pressing the "EFFECT" button on the 
// yamaha remote.
//
// All dsp mode settings are input source specific and normally retained in the
// standby power state.  Nonetheless, all input source DSP settings are reset
// during initialization to insure a known state.

struct dsp { // digital sound processor
    int inputSource;
    const int program;
    int state;            // DSP_EFFECT_ON or DSP_EFFECT_OFF
    const int toggle;     // whether state can change after init
} dsp[] = {

    {DVD,                // DVD always uses Dolby Normal
     DSP_DOLBY_NORMAL,
     DSP_EFFECT_ON,
     DSP_EFFECT_TOGGLE_DISABLE},
     
    {FM,                 // TUNER always has the DSP Effect Off
     DSP_DOLBY_NORMAL,
     DSP_EFFECT_OFF,
     DSP_EFFECT_TOGGLE_DISABLE},
     
    {GEORGIA,            // CD uses DSP_OFF for stereo and Dolby Normal when the surround speakers enabled
     DSP_DOLBY_NORMAL,
     DSP_EFFECT_OFF,
     DSP_EFFECT_TOGGLE_ENABLE},
     
    {JIMSON,             // CBL/SAT uses DSP_OFF for stereo and Dolby Normal when the surround speakers enabled
     DSP_DOLBY_NORMAL,
     DSP_EFFECT_OFF,
     DSP_EFFECT_TOGGLE_ENABLE},
     
    {LVR_MM,             // CD-R always uses Dolby Normal
     DSP_DOLBY_NORMAL,
     DSP_EFFECT_ON,
     DSP_EFFECT_TOGGLE_DISABLE},
     
    {DEN_MM,             // V-AUX always uses Dolby Normal
     DSP_DOLBY_NORMAL,
     DSP_EFFECT_ON,
     DSP_EFFECT_TOGGLE_DISABLE},
     
    {TV,                 // D-TV/LD always uses Dolby Normal
     DSP_DOLBY_NORMAL,
     DSP_EFFECT_ON,
     DSP_EFFECT_TOGGLE_DISABLE},
     
    {VIDEO_AUX,          // V-AUX is an unconnected source that always uses Dolby Normal for initialization
     DSP_DOLBY_NORMAL,
     DSP_EFFECT_ON,
     DSP_EFFECT_TOGGLE_DISABLE},
};


struct irCmd {
    int id;
    char * sz;
    int * state;
    int size;   // number of bytes in the codes array
    unsigned char const * codes;
} irCmd[] = { 

    // yamaha irCmds
    {POWER_ON,
     SZ_POWER_ON,
     &powerState,
     sizeof(irPowerOn),
     irPowerOn},
     
    {POWER_OFF,
     SZ_POWER_OFF,
     &powerState,
     sizeof(irPowerOff),
     irPowerOff},
     
    {GEORGIA,
     SZ_GEORGIA,
     &inputSource,
     sizeof(irCD),
     irCD},
     
    {JIMSON,
     SZ_JIMSON,
     &inputSource,
     sizeof(irCblSat),
     irCblSat},
     
    {LVR_MM,
     SZ_LVR_MM,
     &inputSource,
     sizeof(irCDR),
     irCDR},
     
    {DEN_MM,
     SZ_DEN_MM,
     &inputSource,
     sizeof(irVideoAux),
     irVideoAux},
     
    {FM,
     SZ_FM,
     &inputSource,
     sizeof(irFm),
     irFm},
     
    {TV,
     SZ_TV,
     &inputSource,
     sizeof(irTv),
     irTv},
     
    {DVD,
     SZ_DVD,
     &inputSource,
     sizeof(irDvd),
     irDvd},
     
    {VIDEO_AUX,
     SZ_VIDEO_AUX,
     &inputSource,
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
     
    {DSP_DOLBY_NORMAL,
     SZ_DSP_DOLBY_NORMAL,
     &undefined,
     sizeof(irDspDolbyNormal),
     irDspDolbyNormal},
     
     // xantech irCmds
    {CHANNEL_A_TO_FRONT_SPEAKERS,
     SZ_CHANNEL_A_TO_FRONT_SPEAKERS,
     &channelA_Switch,
     sizeof(irOff0),
     irOff0},
     
    {CHANNEL_A_TO_AB_SWITCH,
     SZ_CHANNEL_A_TO_AB_SWITCH,
     &channelA_Switch,
     sizeof(irOn0),
     irOn0},
     
    {REAR_SPEAKERS_FROM_AB_SWITCH,
     SZ_REAR_SPEAKERS_FROM_AB_SWITCH,
     &rearSpeakerSwitch,
     sizeof(irOff1),
     irOff1},
     
    {REAR_SPEAKERS_FROM_REAR_CHANNEL,
     SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL,
     &rearSpeakerSwitch,
     sizeof(irOn1),
     irOn1},
     
    {CHANNEL_A_RELAY_OFF,
     SZ_CHANNEL_A_RELAY_OFF,
     &channelA_Relay,
     sizeof(irOff2),
     irOff2},
     
    {CHANNEL_A_RELAY_ON,
     SZ_CHANNEL_A_RELAY_ON,
     &channelA_Relay,
     sizeof(irOn2),
     irOn2},
     
    {CHANNEL_B_SPEAKERS_OFF,
     SZ_CHANNEL_B_SPEAKERS_OFF,
     &denSpeakerState,
     sizeof(irOff3),
     irOff3},
     
    {CHANNEL_B_SPEAKERS_ON,
     SZ_CHANNEL_B_SPEAKERS_ON,
     &denSpeakerState,
     sizeof(irOn3),
     irOn3},
     
    {SUBWOOFER_OFF,
     SZ_SUBWOOFER_OFF,
     &subwooferState,
     sizeof(irOff4),
     irOff4},
     
    {SUBWOOFER_ON,
     SZ_SUBWOOFER_ON,
     &subwooferState,
     sizeof(irOn4),
     irOn4},
     
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
     
    {EXIT,
     SZ_EXIT,
     &undefined,
     0,
     NULL}
};    


void initDefaultInputSourceDspProgram() {
    int i;
    printf("\n");    
    for (i=0; i<sizeof(dsp)/sizeof(struct dsp); i++) {
            irTransmit(dsp[i].inputSource);
            irTransmit(dsp[i].program);
            if (dsp[i].state == DSP_EFFECT_OFF)
                irTransmit(DSP_EFFECT_OFF);
            printf("\n");    
    }
} 


int irTransmit(int cmd) {

    int result=0, i;
    char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
    char menuCmd = cmd>=MENU_RANGE_BEGIN && cmd<=MENU_RANGE_END;
    
    if (!volumeCmd && !menuCmd) {
        printf("Transmitting IR command: %s\n", cmdIdToSz(cmd));
    }
    
    struct irCmd * irCmd = cmdIdToIrCmd(cmd);
    
    if (irCmd && irCmd->codes != NULL) {
        memcpy((HGLOBAL)Data, irCmd->codes, irCmd->size);
        
        //if (!initializing)
        //    printIrCmd(irCmd);
        
        delay(cmd);
        
        result = tira_transmit(0, -1, Data, irCmd->size);
        
        if (result) {
            printf("irTransmit: tira_transmit of the %s cmd failed. errno %d\n", irCmd->sz, result);
        }   
        else {
            if (!initializing && (volumeCmd || menuCmd)) {
                printf(".");
                fflush(stdout);
            } 
        }    
        
    } else {
        printf("irTransmit: No IR code for IR command: %s\n", cmdIdToSz(cmd));
        result = -1;
    }    
    
    delay(cmd);
    
    if (!result && !volumeCmd && !menuCmd)
        setState(cmd);
        
    return result;
}


void delay(int cmd) {
    
    if (cmd>=DSP_MODE_CMD_RANGE_BEGIN && cmd<=DSP_MODE_CMD_RANGE_END)
        Sleep(DSP_CMD_DELAY);
    
    if (cmd>=XANTECH_CMD_RANGE_BEGIN && cmd<=XANTECH_CMD_RANGE_END)
        Sleep(XANTECH_DELAY);    // required for command distinction
        
    if (cmd==POWER_ON && powerState==POWER_OFF ||  // allow amplifier time to power up before sending commands
        cmd==POWER_OFF && powerState==POWER_ON) {  // and finish processing commands before powering down
        Sleep(POWER_ON_OFF_DELAY);   
    }    
}           


int getDspState() {
    struct dsp * dsp = getDsp();
    return (dsp ? dsp->state : UNDEFINED);
}    


// broadcast a message to all the audio control clients
void broadcastMsg(char * msg) {
    // Fill in a broadcast address structure to update all the 
    // servers listening on the AUDIO_CONTROL_NOTIFICATION_PORT
    SOCKADDR_IN saServer;
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    //saServer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    saServer.sin_port = htons(AUDIO_CONTROL_NOTIFICATION_PORT);  
    datagramSend(msg, (LPSOCKADDR) &saServer);
    saServer.sin_port = htons(AUDIO_EFFECT_CONTROL_NOTIFICATION_PORT);  
    datagramSend(msg, (LPSOCKADDR) &saServer);
    saServer.sin_port = htons(AUDIO_REMOTE_CONTROL_NOTIFICATION_PORT);  
    datagramSend(msg, (LPSOCKADDR) &saServer);
    printf("Broadcasting msg: %s\n", msg);
    Sleep(50);   // pause to avoid an overrun   
}    
    

// send a message to the requested audio control client
void datagramSend(char * msg, LPSOCKADDR serverAddr) {

    int nRet, flags=0;
        
    nRet = sendto(txSocket,                    
                  msg,                       
                  strlen(msg),               
                  flags,                     
                  serverAddr,                
                  sizeof(struct sockaddr));  
                  
    //printf("datagramSend: %s\n", msg);
    if (nRet == SOCKET_ERROR) {
        printWinsockError("sendto");
    }
}


void printWinsockError(char * errorMsg) {
    printf("\n%s: winsock error code %d\n", errorMsg, WSAGetLastError()); 
}    

     
void setState(int cmd) {

    char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
    char menuCmd = cmd>=MENU_RANGE_BEGIN && cmd<=MENU_RANGE_END;
    char matrixCmd = cmd>=MATRIX_CMD_RANGE_BEGIN && cmd<=MATRIX_CMD_RANGE_END;
    char dspCmd = cmd>=DSP_MODE_CMD_RANGE_BEGIN && cmd<=DSP_MODE_CMD_RANGE_END;
    
    struct irCmd * irCmd = cmdIdToIrCmd(cmd);
    
    switch (cmd) {
        case MUTE_TOGGLE:
            muteState = (cmd==MUTE_OFF) ?  MUTE_ON : MUTE_OFF;
            break;
        case POWER_OFF:
        case POWER_ON:
        case GEORGIA:   
        case JIMSON:   
        case LVR_MM:
        case DEN_MM:
        case FM:
        case TV:   
        case DVD:
        case VIDEO_AUX:
            setState(MUTE_OFF);
        default:
            if (irCmd && irCmd->state!=&undefined)
                *(irCmd->state) = cmd;
            break;
    }
    
    if (dspCmd) {
        struct dsp * dsp = getDsp();
        if (dsp && !initializing) {
            if (cmd==DSP_EFFECT_ON || cmd==DSP_EFFECT_OFF || cmd==DSP_EFFECT_TOGGLE)
               dsp->state = (dsp->state==DSP_EFFECT_ON) ? DSP_EFFECT_OFF : DSP_EFFECT_ON;
        }       
    }   
    
    rearSpeakerState = (channelA_Relay==CHANNEL_A_RELAY_ON) &&
                               (channelA_Switch==CHANNEL_A_TO_AB_SWITCH) &&
                               (rearSpeakerSwitch==REAR_SPEAKERS_FROM_AB_SWITCH) ||
                               (rearSpeakerSwitch==REAR_SPEAKERS_FROM_REAR_CHANNEL) ?
                               REAR_SPEAKERS_ON : REAR_SPEAKERS_OFF;
    frontSpeakerState = (channelA_Relay==CHANNEL_A_RELAY_ON) &&
                        (channelA_Switch==CHANNEL_A_TO_FRONT_SPEAKERS) ?
                        FRONT_SPEAKERS_ON : FRONT_SPEAKERS_OFF;
                        
    if (!matrixCmd && !volumeCmd && !menuCmd && !dspCmd && !initializing && !preserved) {
        // MATRIX, VOLUME, MENU and DSP commands handle broadcast updates on their own
        // updates are withheld and state information is preserved when a client 
        // powers off without deregistering
        broadcastMsg(cmdIdToSz(cmd));
    }                                          
    
}


BOOL isValidCmdId(int cmd) {

    int i;
    
    for (i=0; i<sizeof(irCmd)/sizeof(struct irCmd); i++) {
        if (cmd==irCmd[i].id)
            return TRUE;
    }    
    return FALSE;
}    


BOOL isCmdStateSet(int cmd) {

    char dspCmd = cmd>=DSP_MODE_CMD_RANGE_BEGIN && cmd<=DSP_MODE_CMD_RANGE_END;

    if (dspCmd) {
        return getDspState()==cmd;
    } else {    
        int i;
        for (i=0; i<sizeof(irCmd)/sizeof(struct irCmd); i++) {
            if (cmd==irCmd[i].id && cmd==*(irCmd[i].state))
                return TRUE;
        }
    }        
    return FALSE;
}


struct dsp * getDsp() {
    int i;
    for (i=0; i<sizeof(dsp)/sizeof(struct dsp); i++) {
        if (dsp[i].inputSource==inputSource) {
            return &dsp[i];
        }   
    }
    return NULL;
}


int cmdSzToId(char * msg) {

    int i, retval=UNDEFINED;
    char * vol, c=0;
    
    if ((vol=strstr(msg, SZ_VOLUME)) &&
             strstr(msg, SZ_VOLUME_UP)==NULL &&
             strstr(msg, SZ_VOLUME_DOWN)==NULL) {
        c=*(vol+strlen(SZ_VOLUME));    
        *(vol+strlen(SZ_VOLUME)) = 0;    
    }    
    
    for (i=0; i<sizeof(irCmd)/sizeof(struct irCmd); i++) {
        if ( !(strcmp(msg, irCmd[i].sz)) ){
            retval = irCmd[i].id;
            break;
        }    
    }   
    
    if (c) {
        // restore the original string
        *(vol+strlen(SZ_VOLUME))=c;    
    }    
     
    return retval;
}            


char * cmdIdToSz(int cmd) {

    static char szVolume[256], szFormat[256];
    char volumeCmd = cmd>=VOLUME_RANGE_BEGIN && cmd<=VOLUME_RANGE_END;
    struct irCmd * irCmd = cmdIdToIrCmd(cmd);
    char * retval = SZ_UNDEFINED;
    
    if (!irCmd)
        return SZ_UNDEFINED;
        
    if (volumeCmd) {
        // append the volume string with its absolute volume level
        char * vol;
        char * volPattern = SZ_VOLUME;
        strcpy(szFormat, irCmd->sz);
        if (vol=strstr(szFormat, volPattern)) {
            *(vol+strlen(volPattern)) = 0;
            strcat(szFormat, "%d");
            sprintf(szVolume, szFormat, *(irCmd->state));
            retval=szVolume;
        }
    } else {
        retval = irCmd->sz;
    }    
    
    return retval;
}    


struct irCmd * cmdIdToIrCmd(int cmd) {

    int i;
    
    for (i=0; i<sizeof(irCmd)/sizeof(struct irCmd); i++) {
        if (irCmd[i].id==cmd) {
            return &irCmd[i];
        }   
    }
    
    printf("cmdIdToIrCmd: no translation for the \"%d\" command\n", cmd);
    
    return NULL;
}    


void loadTiraDll() {

   HMODULE handle;
   char * dll;
   
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


void printErrorAndExit(char * func) {

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
        
    printf("\n%s error: %s\n", func, lpMsgBuf);    

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw); 
}


void unloadTiraDll() {
    tira_cleanup();
    FreeLibrary(handle);
}
