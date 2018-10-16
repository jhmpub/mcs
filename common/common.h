#ifndef COMMON_H
#define COMMON_H

#include "extern.h"
#include "tira_tx.h"

// access() declaration and defines to circumvent header file conflicts
extern "C" int access( const char *path, int mode );
#define F_OK    0
#define X_OK    1
#define W_OK    2
#define R_OK    4

#define AUDIO_CONTROL_PORT 2200
#define REMOTE_CONTROL_PORT 2201
#define ITACH_CMD_PORT 4998
#define AUDIO_RECEIVER_AGENT_HOSTNAME "jimson"
#define ITACH_AGENT_HOSTNAME "lvr-itach"

#define DEFAULT_TIRA_COM_PORT 5

#define SZ_UNDEFINED "undefined"

// arrow characters
#define CHAR_ARROW_UP    0x18
#define CHAR_ARROW_DOWN  0x19
#define CHAR_ARROW_RIGHT 0x1a
#define CHAR_ARROW_LEFT  0x1b


enum cmdID {
   // audio receiver commands
   UNDEFINED,
   AUDIO_POWER_ON,
   AUDIO_POWER_OFF,
   GEORGIA,   // input: CBL/SAT
   JIMSON,    // input: CD 
   LVR_MM,    // input: CD-R
   DEN_MM,    // input: V-AUX
   FM,        // input: TUNER
   FM1,       // preset station A1
   FM2,       // preset station A2
   FM3,       // preset station A3
   FM4,       // preset station A4
   FM5,       // preset station A5
   FM6,       // preset station A6
   FM7,       // preset station A7
   FM8,       // preset station A8 
   IR_IGNORE, // ir remote command simulated by the itach wifi to ir device ignored by the remote control agent
   MUTE_ON, 
   MUTE_OFF,
   MUTE_TOGGLE, 
   FIRST_REGISTRATION,
   REGISTER, 
   DEREGISTER, 
   REREGISTER, 
   RESTORE_STATE, 
   TV,        // amplifier input: D-TV/LD
   DVD,       // amplifier input: DVD
   DVR, 
   VIDEO_AUX, 
   SURROUND_MIN,        
   SURROUND_STD,        
   SURROUND_MAX,

   // remote commands
   ARROW_DOWN,          // TV: tv down, DVD: dvd down, DVR: keyboard down
   ARROW_UP,            // TV: tv up, DVD: dvd up, DVR: keyboard up
   ARROW_LEFT,          // DVR: 
   ARROW_RIGHT,         // DVR: "large" handled by processClientCmd
   CHANNEL_DOWN,        // tv mode only
   CHANNEL_UP,          // tv mode only
   EJECT,               //
   EXIT_APPLICATION,    // exit the running application (e.g. windows media center, vlc, internet explorer, etc)
   FAST_FORWARD,        // DVR: jump DVD: >>
   GET_STATE,           // reply with button state information
   INFO,                // DVD: display, STB: info
   LAUNCH_EXPLORER,     // call launchExplorer() with an optional path argument
   MENU,                // TV: menu, DVD: eject, DVR: setNextForegroundWindow()
   MUTE,
   N0,
   N1,
   N2,
   N3,
   N4,
   N5,
   N6,
   N7,
   N8,
   N9,
   NEXT_WINDOW,         // DVR: setNextForegroundWindow()
   OK,                  // TV: vcr ok->tv ok, DVD: vcr ok->dvd ok, DVR: vcr ok->KBD_ENTER
   PAUSE,               // play/pause toggle
   PLAY,                // play/pause toggle if an appID video is running, DVD: dvd ok, DVR: KBD_ENTER
   RECALL,              // recall previous channel SZ_PREVIOUS_CHANNEL
   VIDEO_POWER,         // DVD_MODE ? toggle tv and dvd : toggle tv
   REFRESH_EXPLORER,    // DVR: ctrl-r
   RETURN,
   REWIND,              // DVR: jump DVD: <<
   STATUS,
   STOP,
   SURROUND,
   MODE,                // argument dvd, dvr, tv or round robin if no argument
   VOLUME_DOWN,
   VOLUME_UP,
   
   // hdmi commands
   HDMI_SET_TOP_BOX_SOURCE,
   HDMI_DVD_SOURCE,
   HDMI_DVR_SOURCE,

   // zigbee remote control commands
   CHANNEL,
   DISPLAY_INFO,
   EXIT_PAGE,
   GUIDE,
   PREVIOUS_CHANNEL,
   PAGE_UP,
   PAGE_DOWN,
   NEXT_FAVORITE,
   
   // internal commands
   AR_FIRST_REGISTRATION,   // notice from audio receiver after sending REGISTER request
   AR_MODE,                 // notice from audio receiver to change source
   FEEDBACK_TV_MENU,        // flash tv menu to inform user mode change request queued
   FEEDBACK_TV_RETURN,      // erase tv menu informing user mode change request queued
   
   // general commands
   EXIT_PROCESS        // shutdown running threads, close connections and exit this process
};

extern const char * tira_dll_absolute_path;

BOOL isTiraCmdStateSet(int);
BOOL isValidCmdId(int);

const char * tiraCmdIdToSz(int);
int tiraCmdSzToId(const char *);
int getVolumeLevel(const char *);
int tiraTransmit(int);
int tiraTransmit(struct tiraCmd *);
struct tiraCmd * tiraCmdIdToTiraCmd(int);
struct tiraCmd * tiraCmdSzToTiraCmd(const char *);
void help(void);
void initDefaultInputSourceDspProgram(void);
int initTiraRemoteControl(void);
int initTiraRemoteControl(int);
void setState(const char *);
void loadTiraDll(void);
void processClientMsg(const char *);
void processClientMsgThreadStart(void);
DWORD processClientMsgThread(LPVOID);
void deregisterClient(struct socketDescriptor *);
void unloadTiraDll(void);
void updateState(const char *);

#endif  // ifndef COMMON_H
