#ifndef COMMON_H
#define COMMON_H

#define MEDIA_CONTROL_PORT 2000
#define REMOTE_CONTROL_PORT 2001
#define ITACH_CMD_PORT 4998
#define AUDIO_RECEIVER_AGENT_HOSTNAME "jimson"
#define AUDIO_RECEIVER_DEBUG_HOSTNAME "georgia"
#define ITACH_AGENT_HOSTNAME "lvr-itach"

#define DEFAULT_COM_PORT 5

#define SZ_UNDEFINED "undefined"


class LinkedList {
   public: 
      LinkedList * nextWrap() { return (n->n==NULL) ? f : n; }
      LinkedList * prevWrap() { return (p==NULL) ? l : p; }
      LinkedList * next() { return (n==NULL) ? this : n; }
      LinkedList * prev() { return (p==NULL) ? this : p; }
      LinkedList * first() { return f; }
      LinkedList * last() { return f->l; }
      void * get() { return ptr; }
      void ** getPtr() { return &ptr; }
      void add(void * ptr) { 
         if (n==NULL)
            n = new LinkedList(f, this); 
         this->ptr=ptr;
      }      
      void set(void * ptr) { if (n!=NULL) this->ptr=ptr; }
      BOOL isNotFirst() { return p!=NULL; }
      BOOL isNotLast() { return n!=NULL; }
      BOOL isFirst() { return p==NULL; }
      BOOL isLast() { return n==NULL; }
      LinkedList() {
         f=this;
         l=this;
         p=NULL;
         n=NULL;
         ptr=NULL;
      }
   private:   
      LinkedList * f;
      LinkedList * l;
      LinkedList * p;
      LinkedList * n;
      void * ptr;
      LinkedList(LinkedList * first, LinkedList * p) {
         first->l = p;
         this->f = first;
         this->p = p;
         this->n = NULL;
         this->ptr = NULL;
      }   
};

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
   IR_IGNORE,
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
   EJECT,
   EXIT_APPLICATION,
   FAST_FORWARD,        // DVR: jump DVD: >>
   GET_STATE,           // reply with button state information
   LAUNCH_EXPLORER,     // call launchExplorer() with an optional path argument
   MENU,                // TV: menu, DVD: menu, DVR: setNextForegroundWindow()
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
   
   // general commands
   EXIT
};

// audio receiver command messages
#define SZ_AUDIO_POWER_ON "audio power on"
#define SZ_AUDIO_POWER_OFF "audio power off"
#define SZ_GEORGIA "georgia"
#define SZ_JIMSON "jimson"
#define SZ_LVR_MM "lvr-mm"
#define SZ_DEN_MM "den-mm"
#define SZ_FM  "fm"
#define SZ_FM1 "fm1"
#define SZ_FM2 "fm2"
#define SZ_FM3 "fm3"
#define SZ_FM4 "fm4"
#define SZ_FM5 "fm5"
#define SZ_FM6 "fm6"
#define SZ_FM7 "fm7"
#define SZ_FM8 "fm8"
#define SZ_MUTE_ON "mute on"
#define SZ_MUTE_OFF "mute off"
#define SZ_MUTE_TOGGLE "mute toggle"
#define SZ_REGISTER "register"
#define SZ_DEREGISTER "deregister"
#define SZ_REREGISTER "reregister"
#define SZ_RESTORE_STATE "restore state"
#define SZ_TV  "tv"
#define SZ_DVD  "dvd"
#define SZ_VIDEO_AUX "video aux"
#define SZ_FIRST_REGISTRATION "first registration"
#define SZ_ENABLE_CONTROLS "enable controls"
#define SZ_SURROUND_MIN  "surround min"
#define SZ_SURROUND_STD  "surround std"
#define SZ_SURROUND_MAX  "surround max"

// ir remote control command messages
#define SZ_ARROW_DOWN       "arrow down"
#define SZ_ARROW_LEFT       "arrow left"
#define SZ_ARROW_UP         "arrow up"
#define SZ_ARROW_RIGHT      "arrow right"   
#define SZ_CHANNEL_DOWN     "channel down"
#define SZ_CHANNEL_UP       "channel up"
#define SZ_FAST_FORWARD     "fast forward"
#define SZ_MENU             "menu"
#define SZ_MUTE             "mute"
#define SZ_NEXT_WINDOW      "next window"
#define SZ_N0               "n0"
#define SZ_N1               "n1"
#define SZ_N2               "n2"
#define SZ_N3               "n3"
#define SZ_N4               "n4"
#define SZ_N5               "n5"
#define SZ_N6               "n6"
#define SZ_N7               "n7"
#define SZ_N8               "n8"
#define SZ_N9               "n9"
#define SZ_OK               "ok"
#define SZ_PAUSE            "pause"
#define SZ_PLAY             "play"
#define SZ_VIDEO_POWER      "video power"
#define SZ_RETURN           "return"
#define SZ_REWIND           "rewind"
#define SZ_STOP             "stop"
#define SZ_MODE             "mode"  // dvd, dvr, tv or round robin if no argument

// smart remote control command messages
#define SZ_DVR              "dvr"
#define SZ_EJECT            "eject"
#define SZ_EXIT_APPLICATION "exit application"
#define SZ_GET_STATE        "get state"
#define SZ_LAUNCH_EXPLORER  "launch explorer"  // optional path argument
#define SZ_REFRESH_EXPLORER "refresh explorer"
#define SZ_STATUS           "status"
#define SZ_SURROUND         "surround" // on or off

// hdmi switch remote control command messages
#define SZ_HDMI_SET_TOP_BOX_SOURCE   "hdmi tv source"
#define SZ_HDMI_DVD_SOURCE  "hdmi dvd source"
#define SZ_HDMI_DVR_SOURCE  "hdmi dvr source"

// common commands
#define SZ_EXIT            "exit"


struct tiraCmd {
   int id;
   const char * sz;
   int * state;
   int size;   // number of bytes in the codes array
   const unsigned char * codes;
}; 

extern const char * tira_dll_absolute_path;
extern const char * moduleName;

BOOL consoleModeExitHandler(DWORD);
BOOL isTiraCmdStateSet(int);
BOOL isValidCmdId(int);
BOOL isConsoleMode();
HANDLE initExitHandler(void);
HWND findWindow(const char *);
LONG isLocked(volatile LONG *);
LRESULT WINAPI exitHandler(HWND, UINT, WPARAM, LPARAM);
LPVOID getLastErrorMsg(void);

const char * tiraCmdIdToSz(int);
char * szToLower(char *);
int tiraCmdSzToId(const char *);
int getVolumeLevel(const char *);
int tiraTransmit(int);
int tiraTransmit(struct tiraCmd *);
int ttyLoop(void);
int processArgs(int, char **);
struct tiraCmd * tiraCmdIdToTiraCmd(int);
struct tiraCmd * tiraCmdSzToTiraCmd(const char *);
void createProcess(const char *, const char *);
void help(void);
void initConsoleModeExitHandler(void);
void initDefaultInputSourceDspProgram(void);
void setState(const char *);
void dispatchThread(void);
void loadTiraDll(void);
void printErrorAndExit(char *);
void printErrorMsg(char *);
void printWinsockError(char *);
void processClientCmd(const char *);
void processClientDisconnect();
void processClientDisconnect(struct socketDescriptor *);
void stty_buffered(void);
void stty_unbuffered(void);
void unloadTiraDll(void);
void unlock(volatile LONG *);
void updateState(const char *);
void usage(void);

#endif  // ifndef COMMON_H
