#include "itach_codes.h"
#include "tira_codes.h"

#define MAIN_MODULE "rcAgent"      
#define VIDEO_POWER_ON_DELAY 8000   // milliseconds
#define SZ_VIDEO_DEFAULT_MASTER_VOLUME "44"
#define MODE_MENU MODE

#define  OFF 0
#define  ON  1        

////
//// shortcut keys to synthesize keyboard input
////

// chapter and large jumps are handled by processClientCmd cases for 
// "arrow left large" and "arrow right large" 
// 9 Shift-left or Shift-right arrows for NETFLIX
// Shift-N or Shift-P for VLC
// 3 Ctrl-F, 13 Ctrl-B for WMC
// 12 left or right arrows for YOUTUBE
#define  KBD_SHIFT_N           0x4E10   // Menu -> VLC next chapter 
#define  KBD_SHIFT_P           0x5010   // Menu <- VLC previous chapter
                                        // 
#define  KBD_CTRL_B            0x4211   // << -> WMC 7 seconds back
#define  KBD_LEFT_ARROW        0x0025   // << -> YOUTUBE 5 seconds back
#define  KBD_LEFT_ARROW_CTRL   0x2511   // << -> VLC 60 seconds back
#define  KBD_LEFT_ARROW_SHIFT  0x2510   // << -> NETFLIX 10 seconds back
                                        // 
#define  KBD_CTRL_F            0x4611   // >> -> WMC 30 seconds forward         
#define  KBD_RIGHT_ARROW       0x0027   // >> -> YOUTUBE 5 seconds forward
#define  KBD_RIGHT_ARROW_SHIFT 0x2710   // << -> NETFLIX 10 seconds forward
#define  KBD_RIGHT_ARROW_CTRL  0x2711   // >> -> VLC 60 seconds forward
                                        // 
#define  KBD_CTRL_P            0x5011   // > or " WMC play/pause
#define  KBD_SPACE             0x0020   // > or " NETFLIX, VLC, YOUTUBE play/pause
                                        // 
#define  KBD_ALT_TAB           0x0912   // VCR Menu -> Alt-Tab
#define  KBD_ARROW_DOWN        0x0028   // Down Menu Arrow -> Down Keyboard Arrow
#define  KBD_ARROW_UP          0x0026   // Up Menu Arrow -> Up Keyboard Arrow
#define  KBD_ENTER             0x000D   // > -> Enter
#define  KBD_ALT_F4            0x7312   // Stop -> Alt-F4 
#define  KBD_N0                0x0030   // 0 -> YOUTUBE  0%
#define  KBD_N1                0x0031   // 1 -> YOUTUBE 10%
#define  KBD_N2                0x0032   // 2 -> YOUTUBE 20%
#define  KBD_N3                0x0033   // 3 -> YOUTUBE 30%
#define  KBD_N4                0x0034   // 4 -> YOUTUBE 40%
#define  KBD_N5                0x0035   // 5 -> YOUTUBE 50%
#define  KBD_N6                0x0036   // 6 -> YOUTUBE 60%
#define  KBD_N7                0x0037   // 7 -> YOUTUBE 70%
#define  KBD_N8                0x0038   // 8 -> YOUTUBE 80%
#define  KBD_N9                0x0039   // 9 -> YOUTUBE 90%
#define  KBD_CTRL_R            0x5211   // prepends remote Up, Down, VCR Menu, TV PIP -> Ctrl-R

#define  SZ_KBD_SHIFT_N             "shift n "
#define  SZ_KBD_SHIFT_P             "shift p"
                                    
#define  SZ_KBD_CTRL_B              "ctrl b"
#define  SZ_KBD_LEFT_ARROW          "left arrow"
#define  SZ_KBD_LEFT_ARROW_CTRL     "ctrl left arrow"
#define  SZ_KBD_LEFT_ARROW_SHIFT    "shift left arrow"
                               
#define  SZ_KBD_CTRL_F              "ctrl f"
#define  SZ_KBD_RIGHT_ARROW         "right arrow"
#define  SZ_KBD_RIGHT_ARROW_SHIFT   "ctrl right arrow"
#define  SZ_KBD_RIGHT_ARROW_CTRL    "shift right arrow"
                               
#define  SZ_KBD_CTRL_P              "ctrl p"
#define  SZ_KBD_SPACE               "space"
                               
#define  SZ_KBD_ALT_TAB             "alt tab"
#define  SZ_KBD_ARROW_DOWN          "arrow down"
#define  SZ_KBD_ARROW_UP            "arrow up"
#define  SZ_KBD_ENTER               "enter"
#define  SZ_KBD_ALT_F4              "alt f4"
#define  SZ_KBD_N0                  "N0"
#define  SZ_KBD_N1                  "N1"
#define  SZ_KBD_N2                  "N2"
#define  SZ_KBD_N3                  "N3"
#define  SZ_KBD_N4                  "N4"
#define  SZ_KBD_N5                  "N5"
#define  SZ_KBD_N6                  "N6"
#define  SZ_KBD_N7                  "N7"
#define  SZ_KBD_N8                  "N8"
#define  SZ_KBD_N9                  "N9"
#define  SZ_KBD_CTRL_R              "ctrl r"

enum appID {
   NETFLIX,
   VLC,
   WMC,
   YOUTUBE,  // nb click once on a running video to pause and set hotkey focus
   APP_SIZE,
   APP_UNKNOWN
};

struct appTitle {
    const char * sz;
    int id;
} appTitle[] = {    
   { "Netflix - ", NETFLIX },
   { "Microsoft Silverlight", NETFLIX },
   { "VLC media player", VLC },
   { "Windows Media Center", WMC },
   { "YouTube - ", YOUTUBE }
}; 

struct shortcut {
   unsigned vkCode[APP_SIZE];    // indexed by appID enum
   const char * sz[APP_SIZE];    // debug description
};   

static struct shortcut scLgJumpBackward = { 
   {
      KBD_LEFT_ARROW_SHIFT,
      KBD_SHIFT_P,
      KBD_CTRL_B,
      KBD_LEFT_ARROW
   },
   {
      SZ_KBD_LEFT_ARROW_SHIFT,
      SZ_KBD_SHIFT_P,
      SZ_KBD_CTRL_B,
      SZ_KBD_LEFT_ARROW
   }   
};  

static struct shortcut scLgJumpForward = { 
   {
      KBD_RIGHT_ARROW_SHIFT,
      KBD_SHIFT_N,
      KBD_CTRL_F,
      KBD_RIGHT_ARROW
   },
   {
      SZ_KBD_RIGHT_ARROW_SHIFT,
      SZ_KBD_SHIFT_N,
      SZ_KBD_CTRL_F,
      SZ_KBD_RIGHT_ARROW
   }   
};

static struct shortcut scSmJumpBackward = { 
   {
      KBD_LEFT_ARROW_SHIFT,
      KBD_LEFT_ARROW_CTRL,
      KBD_CTRL_B,
      KBD_LEFT_ARROW
   },
   {
      SZ_KBD_LEFT_ARROW_SHIFT,
      SZ_KBD_LEFT_ARROW_CTRL,
      SZ_KBD_CTRL_B,
      SZ_KBD_LEFT_ARROW
   }   
};  

static struct shortcut scSmJumpForward = { 
   {
      KBD_RIGHT_ARROW_SHIFT,
      KBD_RIGHT_ARROW_CTRL,
      KBD_CTRL_F,
      KBD_RIGHT_ARROW
   },
   {
      SZ_KBD_RIGHT_ARROW_SHIFT,
      SZ_KBD_RIGHT_ARROW_CTRL,
      SZ_KBD_CTRL_F,
      SZ_KBD_RIGHT_ARROW
   }   
};

static struct shortcut scPlayPause = {
   {
      KBD_SPACE,
      KBD_SPACE,
      KBD_CTRL_P,
      KBD_SPACE
   },
   {
      SZ_KBD_SPACE,
      SZ_KBD_SPACE,
      SZ_KBD_CTRL_P,
      SZ_KBD_SPACE
   }   
};

static struct shortcut scAltTab = {
   {KBD_ALT_TAB}, {SZ_KBD_ALT_TAB}
};

static struct shortcut scArrowUp = {
   {KBD_ARROW_UP}, {SZ_KBD_ARROW_UP}
};

static struct shortcut scArrowDown = {
   {KBD_ARROW_DOWN}, {SZ_KBD_ARROW_DOWN}
};

static struct shortcut scEnter = {
   {KBD_ENTER}, {SZ_KBD_ENTER}
};

static struct shortcut scAltF4 = {
   {KBD_ALT_F4}, {SZ_KBD_ALT_F4}
};

static struct shortcut scCtrlR = {
   {KBD_CTRL_R}, {SZ_KBD_CTRL_R}
};

static struct shortcut scN0 = {
   {KBD_N0}, {SZ_KBD_N0}
};

static struct shortcut scN1 = {
   {KBD_N1}, {SZ_KBD_N1}
};

static struct shortcut scN2 = {
   {KBD_N2}, {SZ_KBD_N2}
};

static struct shortcut scN3 = {
   {KBD_N3}, {SZ_KBD_N3}
};

static struct shortcut scN4 = {
   {KBD_N4}, {SZ_KBD_N4}
};

static struct shortcut scN5 = {
   {KBD_N5}, {SZ_KBD_N5}
};

static struct shortcut scN6 = {
   {KBD_N6}, {SZ_KBD_N6}
};

static struct shortcut scN7 = {
   {KBD_N7}, {SZ_KBD_N7}
};

static struct shortcut scN8 = {
   {KBD_N8}, {SZ_KBD_N8}
};

static struct shortcut scN9 = {
   {KBD_N9}, {SZ_KBD_N9}
};


//// rcCmd struct translates ir and tcp input messages to output synthesized 
//// keyboard shortcuts and/or itach wifi to ir commands 
static struct rcCmd {
   int id;                            // rcCmdID enum
   const char * sz;                   // tcp command string and description
   struct irRx * irRx;                // ir input
   struct shortcut * shortcut;        // dvr synthesized keyboard output
   struct irTx * irTx;                // wifi to tv or dvd ir output
} rcCmd[] = { 

   {
      UNDEFINED,
      SZ_UNDEFINED,
      &irRxUndefined,
      NULL,
      NULL
   }, 
   
   {
      ARROW_DOWN,
      SZ_ARROW_DOWN,
      &irRxVcrMenuDown,
      &scArrowDown,
      irTxMenuDown
   },
   
   {
      ARROW_LEFT,
      SZ_ARROW_LEFT,
      &irRxVcrMenuLeft,
      &scLgJumpBackward,
      irTxMenuLeft,
   },
   
   {
      ARROW_RIGHT,
      SZ_ARROW_RIGHT,
      &irRxVcrMenuRight,
      &scLgJumpForward,
      irTxMenuRight,
   },
    
   {
      ARROW_UP,
      SZ_ARROW_UP,
      &irRxVcrMenuUp,
      &scArrowUp,
      irTxMenuUp
   },
   
   {
      CHANNEL_DOWN,
      SZ_CHANNEL_DOWN,
      &irRxVcrChannelDown,
      NULL,
      irTxChannelDown
   },
   
   {
      CHANNEL_UP,
      SZ_CHANNEL_UP,
      &irRxVcrChannelUp,
      NULL,
      irTxChannelUp
   },
   
   {
      DVD,
      SZ_DVD,
      NULL,
      NULL,
      irTxN2
   },
   
   {
      DVR,
      SZ_DVR,
      NULL,
      NULL,
      irTxN4
   },
    
   {
      EJECT,
      SZ_EJECT,
      &irRxDvdEject,
      NULL,
      irTxEject
   },
   
   {
      EXIT,
      SZ_EXIT,
      NULL,
      NULL,
      NULL
   },
    
   {
      EXIT_APPLICATION,
      SZ_EXIT_APPLICATION,
      NULL,
      &scAltF4,
      NULL
   },
   
   {
      FAST_FORWARD,
      SZ_FAST_FORWARD,
      &irRxVcrFastForward,
      &scSmJumpForward,
      irTxFastForward
   },
   
   {
      GET_STATE,
      SZ_GET_STATE,
      NULL,
      NULL,
      NULL
   },
    
   {
      LAUNCH_EXPLORER,
      SZ_LAUNCH_EXPLORER,
      NULL,
      NULL,
      NULL
   },
   
   {
      REFRESH_EXPLORER,
      SZ_REFRESH_EXPLORER,
      NULL,
      &scCtrlR,
      NULL
   },
    
   {
      MENU,
      SZ_MENU,
      &irRxVcrMenu,
      NULL,
      irTxMenu
   },
    
   {
      N0,
      SZ_N0,
      &irRxVcrN0,
      &scN0,
      irTxN0
   },
    
   {
      N1,
      SZ_N1,
      &irRxVcrN1,
      &scN1,
      irTxN1
   },
    
   {
      N2,
      SZ_N2,
      &irRxVcrN2,
      &scN2,
      irTxN2
   },
    
   {
      N3,
      SZ_N3,
      &irRxVcrN3,
      &scN3,
      irTxN3
   },
    
   {
      N4,
      SZ_N4,
      &irRxVcrN4,
      &scN4,
      irTxN4
   },
    
   {
      N5,
      SZ_N5,
      &irRxVcrN5,
      &scN5,
      irTxN5
   },
    
   {
      N6,
      SZ_N6,
      &irRxVcrN6,
      &scN6,
      irTxN6
   },
    
   {
      N7,
      SZ_N7,
      &irRxVcrN7,
      &scN7,
      irTxN7
   },
    
   {
      N8,
      SZ_N8,
      &irRxVcrN8,
      &scN8,
      irTxN8
   },
    
   {
      N9,
      SZ_N9,
      &irRxVcrN9,
      &scN9,
      irTxN9
   },
    
   {
      NEXT_WINDOW,
      SZ_NEXT_WINDOW,
      NULL,
      NULL,
      NULL
   },
    
   {
      OK,
      SZ_OK,
      &irRxVcrOk,
      &scEnter,
      irTxOk
   },
    
   {
      PAUSE,
      SZ_PAUSE,
      &irRxVcrPause,
      &scPlayPause,
      irTxPause
   },
    
   {
      PLAY,
      SZ_PLAY,
      &irRxVcrPlay,
      &scPlayPause,
      irTxPlay
   },
   
   {
      VIDEO_POWER,
      SZ_VIDEO_POWER,
      &irRxVcrPower,
      NULL,
      irTxPower
   },
   
   {
      RESTORE_STATE,
      SZ_RESTORE_STATE,
      NULL,
      NULL,
      NULL
   },
   
   {
      RETURN,
      SZ_RETURN,
      NULL,
      NULL,
      irTxReturn
   },
   
   {
      REWIND,
      SZ_REWIND,
      &irRxVcrRewind,
      &scSmJumpBackward,
      irTxRewind
   },
   
   {
      STOP,
      SZ_STOP,
      &irRxVcrStop,
      &scAltF4,
      irTxStop
   },
   
   {
      TV,
      SZ_TV,
      NULL,
      NULL,
      irTxN4
   },
    
   {
      STATUS,
      SZ_STATUS,
      NULL,
      NULL,
      irTxDisplay
   },
   
   {
      SURROUND,
      SZ_SURROUND,
      NULL,
      NULL,
      NULL
   },
   
   {
      MUTE,
      SZ_MUTE,
      &irRxMute,
      NULL,
      NULL,
   },
    
   {
      MODE,
      SZ_MODE,
      &irRxVcrVideo,
      NULL,
      irTxVideo
   },
    
   {
      VOLUME_DOWN,
      SZ_VOLUME_DOWN,
      &irRxVolumeDown,
      NULL,
      NULL
   },
   
   {
      VOLUME_UP,
      SZ_VOLUME_UP,
      &irRxVolumeUp,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdDisplay,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdFastForward,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdMenu,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdMenuDown,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdMenuLeft,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdMenuRight,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdMenuUp,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN0,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN1,
      NULL,
      NULL
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN2,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN3,
      NULL,
      NULL
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN4,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN5,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN6,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN7,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN8,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdN9,
      NULL,
      NULL
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdOk,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdPause,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdPlay,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdPower,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdRewind,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxDvdStop,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxHdmiSourceInput1,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxHdmiSourceInput2,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxHdmiSourceInput3,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxHdmiSourceToggle,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvChannelDown,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvChannelUp,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvDisplay,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvFastForward,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvMenu,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvMenuDown,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvMenuLeft,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvMenuRight,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvMenuUp,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN0,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN1,
      NULL,
      NULL
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN2,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN3,
      NULL,
      NULL
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN4,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN5,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN6,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN7,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN8,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvN9,
      NULL,
      NULL
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvPause,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvPip,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvPlay,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvPower,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvReturn,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvRewind,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvStop,
      NULL,
      NULL
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxTvVideo,
      NULL,
      NULL
   },
   
};

// enumerate whether an itach command depends on the success of the previous itach command
enum itachDependency {
   INDEPENDENT,
   DEPENDENT
};   

// define the class passed to sendItachCmdThread as an argument 
class ItachCmdParam {
   public:
      struct irTx * irTx;   // itach command structure
      struct irRx * irRx;   // tira irRx structure expected to confirm a successful itach command request
      int dependent;        // TRUE if this command depends on success of the previous command 
      unsigned sequence;    // used to serialize itach commands
      int delay;            // millisecond processing delay allowance
   ItachCmdParam(
      struct irTx * irTx,
      int dependent,
      int sequence,
      int delay) {
      this->irTx = irTx;
      this->irRx = irTxSzToIrRx(irTx->sz);
      this->dependent = dependent;
      this->sequence = sequence;
      this->delay = delay;
      irRx=NULL;
   }
   
   private:
   // return the tira irRx structure that corresonds to an itach irTx
   // command.  The irRx struct is used to confirm a requested itach 
   // transmit is successfully received.
   struct irRx * irTxSzToIrRx(const char * sz) {
      int i;
      char * match;
      
      // search for a partial end of string match
      // e.g. "tv channel down" -> "ir tv channel down"
      for (i=0; i<sizeof(rcCmd)/sizeof(struct rcCmd); i++) {
         if (rcCmd[i].irRx && 
            (match=strstr(rcCmd[i].irRx->sz, sz)) && 
             strlen(match)==strlen(sz)) {
            return rcCmd[i].irRx;
         }   
      }
      return rcCmd[UNDEFINED].irRx;
   }
};


// global variables used to sequentially process itach commands
static struct {
   struct irRx * irRxPending;
   int irComplete;
   int prerequisiteFailure;
   unsigned sequence;
   char debug[1024];
} itachCmd;


// tiraCmdThread processing definitions and declarations
static unsigned tiraCmdActive=0;
class TiraCmd {
   public:
      const char * sz;
      char debug[1024];
      int duplicate;
      unsigned sequence;
   TiraCmd(
      const char * sz,
      char * debug,
      int duplicate,
      unsigned sequence) {
      this->sz = sz;
      strncpy(this->debug, debug, sizeof(this->debug));
      this->duplicate = duplicate;
      this->sequence = sequence;
   }
};         
     

static struct {
   HANDLE hThread;
   int mode;
} videoChange;       

static SOCKET arAgentSocket;
static struct socketDescriptor arAgentSd = {
   NULL,
   MEDIA_CONTROL_PORT,
   &arAgentSocket,
   FALSE,
   FALSE,
   "arAgent client",
   NULL,
   NULL
};

static SOCKET itachSocket;   
static struct socketDescriptor itachSd = {
   ITACH_AGENT_HOSTNAME,
   ITACH_CMD_PORT,
   &itachSocket,
   FALSE,
   FALSE,
   "itach client",
   NULL,
   NULL
};

const char * tira_dll_absolute_path = "c:\\utils\\startup\\local\\rcagent\\tira2.dll";
const char * moduleName = "rcAgent";

BOOL forceForegroundWindow(HWND);
BOOL isCmdStateSet(int);
BOOL isExplorerForeground(void);
BOOL isAudioFromVideo(void);
char * getForegroundWindowImageFileName(char *);
char * rcCmdIdToSz(int);
const char * vkCodeToSz(unsigned);
DWORD tiraCmdThread(LPVOID);
DWORD rxBroadcastThread(LPVOID);
DWORD sendItachCmdThread(LPVOID);
DWORD menuFlashThread(LPVOID);
DWORD videoModeChangeThread(LPVOID);
int __stdcall irRxCmdCallback(char const *);
int filterCmd(int);
int getForegroundWindowAppID(void);
int getVirtualKeyCode(struct rcCmd *);
int cmdSzToRcCmdId(const char *);
int getRemoteMode(void);
int getVideoMode(int);
int irRxHasCode(struct irRx *, char const *);
struct rcCmd * cmdSzToRcCmd(const char *);
struct rcCmd * cmdIdToRcCmd(int);
void errMsgExit(char *);
void initDefaults(void);
void initClientThread(struct socketDescriptor *);
void launchExplorer(const char *);
void loadTiraDLL(void);
void printErrorAndExit(char *);
void processClientCmd(const char *, TiraCmd *);
void processArAgentNotification(char *);
void restoreReceiverState(void);
void sendArCmd(const char *);
void sendDefault(struct rcCmd *);
void sendItachCmd(int, int);
void sendItachCmd(int, int, int, int);
void sendItachCmdThreadStart(ItachCmdParam *);
void sendVirtualKeyCode(unsigned);
void sendVirtualKeyCode(unsigned, int);
void setAudioMode(int);
void setAudioSource(int);
void setVideoMode(int);
void setVideoSource(int);
void setNextForegroundWindow(void);
void setSurroundSound(int);
void tiraCmdThreadStart(TiraCmd *);
void toggleDvdPower(int);
void toggleVideoPower(void);
void videoModeChangeThreadStart(int);
void menuFlashThreadStart(void);
static char szDefaultMasterVolume[] = SZ_MASTER_VOLUME SZ_VIDEO_DEFAULT_MASTER_VOLUME;
static DWORD irRxTime;
static int displayMode=UNDEFINED, nextMode=UNDEFINED;
static int tvPowered=FALSE, dvdPowered=FALSE;
static int dependentTimeout=FALSE;
static int registered=FALSE;


