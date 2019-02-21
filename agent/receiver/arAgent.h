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
// Sending DSP_DOLBY_NORMAL always sets DSP_EFFECT_ON.
// Sending DSP_DOLBY_NORMAL twice within three seconds toggles between the 
// "Normal" and "Enhanced" sub-programs.
//
// A dsp mode is associated with each input source and retained in the standby
// power state.  During initialization, each input source is set to the defined
// DSP program forcing DSP_EFFECT_ON.  If state is predefined to DSP_EFFECT_OFF
// the initialization routine will subsequently turn the DSP off.

struct dsp { // digital sound processor
   const int audioSource;        // e.g. DVD, TUNER, CD, etc.
   const int program;            // e.g. DSP_DOLBY_NORMAL (5 speaker)
   int state;                    // DSP_EFFECT_OFF disables program
   const int toggleEnable;       // define if state can change after init
} dsp[] = {

   {DVD,                         // DVD input
    DSP_DOLBY_NORMAL,            // HD (five speaker) media 
    DSP_EFFECT_ON,               // rear speaker channels lost if relay off
    DSP_EFFECT_TOGGLE_DISABLE},  // front on+rear on - "Normal", front on+rear off - "Normal"
    
   {FM,                          // TUNER input
    DSP_DOLBY_NORMAL,            // STEREO (two speaker) media
    DSP_EFFECT_OFF,              // front speakers receive all sound
    DSP_EFFECT_TOGGLE_DISABLE},  // front on+rear on - "Effect Off", front on+rear off - "Effect Off"
    
   {GEORGIA,                     // CD input
    DSP_DOLBY_NORMAL,            // HD (five speaker) media will decode fine but       
    DSP_EFFECT_OFF,              // STEREO media will sound distorted/muffled if DSP "Normal"
    DSP_EFFECT_TOGGLE_ENABLE},   // front on+rear on - "Normal", front on+rear off - "Effect Off" 
    
   {JIMSON,                      // CBL/SAT input
    DSP_DOLBY_NORMAL,            // HD (five speaker) media *typically*         
    DSP_EFFECT_OFF,              // STEREO media will sound distorted/muffled if DSP "Normal"
    DSP_EFFECT_TOGGLE_ENABLE},   // front on+rear on - "Normal", front on+rear off - "Effect Off"     
    
   {LVR_MM,                      // CD-R input
    DSP_DOLBY_NORMAL,            // HD (five speaker) media              
    DSP_EFFECT_ON,               // rear speaker channels lost if relay off     
    DSP_EFFECT_TOGGLE_DISABLE},  // front on+rear on - "Normal", front on+rear off - "Normal" 
    
   {DEN_MM,                      // V-AUX input unconnected source
    DSP_DOLBY_NORMAL,            // HD (five speaker) media              
    DSP_EFFECT_ON,               // rear speaker channels lost if relay off     
    DSP_EFFECT_TOGGLE_DISABLE},  // front on+rear on - "Normal", front on+rear off - "Normal"
    
   {TV,                          // D-TV/LD input
    DSP_DOLBY_NORMAL,            // HD (five speaker) media             
    DSP_EFFECT_ON,               // rear speaker channels lost if relay off    
    DSP_EFFECT_TOGGLE_DISABLE},  // front on+rear on - "Normal", front on+rear off - "Normal"
    
   {VIDEO_AUX,                   // V-AUX input unconnected source used for initialization
    DSP_DOLBY_NORMAL,            // HD (five speaker) media             
    DSP_EFFECT_ON,               // rear speaker channels lost if relay off
    DSP_EFFECT_TOGGLE_DISABLE},  // front on+rear on - "Normal", front on+rear off - "Normal"
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
void irSend(int);
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
