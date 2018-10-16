#ifndef MCS_EXTERN
#define MCS_EXTERN

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <windef.h>
#endif

#ifdef __CYGWIN__
#include <apisetcconv.h>
#include <minwinbase.h>
#endif
#include <wincon.h>
#include <malloc.h>

// audio receiver command messages
#define SZ_AUDIO_POWER_ON            "audio power on"
#define SZ_AUDIO_POWER_OFF           "audio power off"
#define SZ_GEORGIA                   "georgia"
#define SZ_JIMSON                    "jimson"
#define SZ_LVR_MM                    "lvr-mm"
#define SZ_DEN_MM                    "den-mm"
#define SZ_FM                        "fm"
#define SZ_FM1                       "fm1"
#define SZ_FM2                       "fm2"
#define SZ_FM3                       "fm3"
#define SZ_FM4                       "fm4"
#define SZ_FM5                       "fm5"
#define SZ_FM6                       "fm6"
#define SZ_FM7                       "fm7"
#define SZ_FM8                       "fm8"
#define SZ_MUTE_ON                   "mute on"
#define SZ_MUTE_OFF                  "mute off"
#define SZ_MUTE_TOGGLE               "mute toggle"
#define SZ_REGISTER                  "register"
#define SZ_DEREGISTER                "deregister"
#define SZ_REREGISTER                "reregister"
#define SZ_RESTORE_STATE             "restore state"
#define SZ_TV                        "tv"
#define SZ_DVD                       "dvd"
#define SZ_VIDEO_AUX                 "video aux"
#define SZ_FIRST_REGISTRATION        "first registration"
#define SZ_ENABLE_CONTROLS           "enable controls"
#define SZ_SURROUND_MIN              "surround min"
#define SZ_SURROUND_STD              "surround std"
#define SZ_SURROUND_MAX              "surround max"

// ir and zigbee remote control command messages
#define SZ_ARROW_DOWN                "arrow down"
#define SZ_ARROW_LEFT                "arrow left"
#define SZ_ARROW_UP                  "arrow up"
#define SZ_ARROW_RIGHT               "arrow right"   
#define SZ_CHANNEL_DOWN              "channel down"
#define SZ_CHANNEL_UP                "channel up"
#define SZ_FAST_FORWARD              "fast forward"
#define SZ_MENU                      "menu"
#define SZ_MUTE                      "mute"
#define SZ_NEXT_WINDOW               "next window"
#define SZ_0                         "0"
#define SZ_1                         "1"
#define SZ_2                         "2"
#define SZ_3                         "3"
#define SZ_4                         "4"
#define SZ_5                         "5"
#define SZ_6                         "6"
#define SZ_7                         "7"
#define SZ_8                         "8"
#define SZ_9                         "9"
#define SZ_OK                        "ok"
#define SZ_PAUSE                     "pause"
#define SZ_PLAY                      "play"
#define SZ_VIDEO_POWER               "video power"
#define SZ_RETURN                    "return"
#define SZ_REWIND                    "rewind"
#define SZ_STOP                      "stop"
#define SZ_MODE                      "mode"  // dvd, dvr, tv or round robin if no argument

// smart remote control command messages
#define SZ_DVR                       "dvr"
#define SZ_EJECT                     "eject"
#define SZ_EXIT_APPLICATION          "exit application"
#define SZ_GET_STATE                 "get state"
#define SZ_LAUNCH_EXPLORER           "launch explorer"  // optional path argument
#define SZ_REFRESH_EXPLORER          "refresh explorer"
#define SZ_STATUS                    "status"
#define SZ_SURROUND                  "surround" // on or off

// hdmi switch remote control command messages
#define SZ_HDMI_SET_TOP_BOX_SOURCE   "hdmi tv source"
#define SZ_HDMI_DVD_SOURCE           "hdmi dvd source"
#define SZ_HDMI_DVR_SOURCE           "hdmi dvr source"

// zigbee remote control command messages
#define SZ_CHANNEL                   "channel"
#define SZ_DISPLAY_INFO              "display info"
#define SZ_EXIT_PAGE                 "exit page"
#define SZ_GUIDE                     "guide"
#define SZ_PREVIOUS_CHANNEL          "previous channel"  // enum cmdID RECALL
#define SZ_PAGE_UP                   "page up"
#define SZ_PAGE_DOWN                 "page down"
#define SZ_NEXT_FAVORITE             "next favorite"

// general commands
#define SZ_EXIT_PROCESS              "exit process"


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


// initializing to the maximum volume levels insures sufficient "down"
// volume commands will be issued during ampInit to reach volume floors
extern BOOL isClientMsgThreadRunning;
extern HMODULE tiraDLL;
extern int masterVolume;
extern int centerVolume;
extern int subwooferVolume;
extern int surroundRightVolume;
extern int surroundLeftVolume;
extern int debug;
extern int exiting;
extern int initializing;
extern int audioSource;
extern int station;
extern int currentEffectSpeaker;
extern int audioPowerState, muteState;
extern int registrationCount, surroundProfile;
extern int channel_a_director, rear_speaker_selector, channel_a_relay;
extern int bSpeakerState, subwoofer_relay, center_speaker_relay;
extern int rearSpeakerState, frontSpeakerState;
extern int subwooferState, subwooferRelayState, LFE_State;
extern int hdmiSwitchState;
extern int undefined;
extern INPUT_RECORD inputQ[3];


extern struct serverState {
   int rearSpeakerState; 
   int frontSpeakerState; 
   int bSpeakerState;
   int audioSource;
} serverState, *preserved;        

struct tiraCmd {
   int id;
   const char * sz;
   int * state;
   int size;   // number of bytes in the codes array
   const unsigned char * codes;
}; 


// used by tira callback to filter erroneous ir codes and print sequential debug
class IrCmd {
   public:
      char debug[1024];
      DWORD timestamp;
      int id;
   IrCmd(char * debug, int id, DWORD timestamp) {
      strncpy(this->debug, debug, sizeof(this->debug));
      this->id = id;
      this->timestamp = timestamp;
   }
};         


// queueing declarations
class QueuedMsg {
   private:
      void init(const char * sz, struct socketDescriptor * sd, IrCmd * irCmd) { 
         this->sz = (char *) malloc(strlen(sz)+1);
         strcpy(this->sz, sz);
         this->sd = sd;
         this->irCmd = irCmd;
      }
   public:
      char * sz;
      struct socketDescriptor * sd;
      IrCmd * irCmd;
      
      QueuedMsg(const char * sz) {
         this->init(sz, NULL, NULL);
      }    
      
      QueuedMsg(const char * sz, struct socketDescriptor * sd) { 
         this->init(sz, sd, NULL);
      }    
      
      QueuedMsg(const char * sz, IrCmd * irCmd) {
         this->init(sz, NULL, irCmd);
      }    
      
      QueuedMsg(const char * sz, struct socketDescriptor * sd, IrCmd * irCmd) { 
         this->init(sz, sd, irCmd);
      }
      
      ~QueuedMsg() { 
         free(sz); 
         if (irCmd)
            delete irCmd;
      }
};      


BOOL isMsgQueueEmpty(void);
void msgQueueAdd(const char *);
void msgQueueAdd(const char *, IrCmd *);
void msgQueueAdd(const char *, struct socketDescriptor *);
QueuedMsg * msgQueueRemove(void);

char * printQueueRemove(void);
void printQueueAdd(char *);

char * szQueueRemove(LinkedList **);
void szQueueAdd(LinkedList **, const char *);

void queueAdd(LinkedList **, void *);
void * queueRemove(LinkedList **);
BOOL isQueueEmpty(LinkedList *);


// semaphore functions
int isLocked(const char *);
LONG isLocked(volatile LONG *);
void unlock(void);
void unlock(volatile LONG *);


// general functions
BOOL consoleModeExitHandler(DWORD);
BOOL isConsoleMode();
char * keyToName(char, char *, int);
char * szToLower(char *);
char getChar(void);
HANDLE getStdinHandle(void);
HANDLE initExitHandler(void);
HWND findWindow(const char *);
int printq(const char *, ...);
int ttyLoop(void);
LRESULT WINAPI exitHandler(HWND, UINT, WPARAM, LPARAM);
void createProcess(const char *, const char *);
void dispatchThread(void);
void errorExit(const char *);
void initConsoleModeExitHandler(void);
void printLoadTiraDllErrorAndExit(const char *);
void processArgs(int, char **);
void tty_buffering(int);
void usage(char *);

#endif  // ifndef MCS_EXTERN



