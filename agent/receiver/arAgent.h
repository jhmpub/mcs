#define MAIN_MODULE "arAgent"      

// The default setting in two speaker mode is DSP_EFFECT_OFF which means
// the center and rear speakers are downmixed into the main speakers.
// DSP_EFFECT_ON is used to upmix stereo and decode surround sound.
//
// Two speaker mode requires "Effect Off" for complete sound.
//
// The receiver should always display "Effect Off" in stereo mode and "Normal" 
// in surround mode.  
//
// All HD sources are hardcoded to use Dolby Normal decoding with DSP_EFFECT_ON.
// The "Enhanced" sub-mode does not seem to utilize the center channel very
// well when the audio source is stereo so "Normal" is the preferred setting in
// surround sound mode.
//
// "Normal" delivers 5 channel DSP disabled sound.
// "Enhanced" delivers 5 channel DSP enabled sound.
// "Effect Off" delivers 2 channel DSP disabled sound.
//
// Sending DSP_EFFECT_TOGGLE alternates between the 2 channel "Effect Off" mode
// and the dsp mode associated with the current input source.
//
// Sending DSP_DOLBY_NORMAL switches from "Effect Off" to "Normal/Enhanced".
// Sending DSP_DOLBY_NORMAL twice within three seconds toggles between the 
// "Normal" and "Enhanced" sub-programs.
//
// A dsp mode is associated with each input source and retained in the standby
// power state.  All input source DSP settings are restored during
// initialization to their default "Effect Off" or "Normal/Enhanced" setting to
// insure a known state.  Unfortunately, it is not possible to assuredly 
// initialize DSP_EFFECT_ON to the "Normal" setting.  DSP_DOLBY_NORMAL retains
// the most recent "Normal" or "Enhanced" setting.

struct dsp { // digital sound processor
   int audioSource;
   const int program;
   int state;                // DSP_EFFECT_ON or DSP_EFFECT_OFF
   const int toggleEnable;   // define if state can change after init
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


BOOL consoleModeExitHandler(DWORD fdwCtrlType);
BOOL isCmdStateSet(int);
BOOL isConsoleMode(void);
BOOL isValidCmdId(int);

char * cmdIdToSz(int);

HANDLE initWindowsModeExitHandler(void);

int getDspMode(void);
int getDspProgram(void);
int getDspEffectState(void);
int isDspEffectToggleEnabled(void);

LRESULT WINAPI windowsModeExitHandler(HWND, UINT, WPARAM, LPARAM);

const char * tira_dll_absolute_path = "c:\\utils\\startup\\local\\acserver\\tira2.dll";

SOCKADDR saClient;

struct dsp * getDsp(void);

void activateSpeakerMenu(const char *);
void ampInit(void);
void broadcastMsg(const char *);
void delay(int);
void dispatchThread(void);
void initConsoleModeExitHandler(void);
void initDefaultInputSourceDspProgram(void);
void initStartupDelay(int);
void irSend(const char *);
void irSend(QueuedMsg *);
void loadTiraDll(void);
void printErrorAndExit(char *);
void printTiraCmd(struct tiraCmd *);
void setInputSourceDspEffect(void);
void setLFE(int);
void setSurroundProfile(int);
void setSurroundProfileByVolume(void);
void setSwitchMatrix(int);
void setVolume(const char *);
void updateState(int);
