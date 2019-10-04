#include "itach.h"
#include "tira_rx.h"
#include "shortcuts.h"

// direct zrcc.h to not compile this module as a controller or a target
#define HOST_CMN TRUE  
#include "zrcc.h"

#define VIDEO_POWER_ON_DELAY 4000   // milliseconds
#define SZ_VIDEO_DEFAULT_MASTER_VOLUME "44"
#define VIDEO_INPUT_MENU MODE

#define  OFF 0
#define  ON  1        

#define SZ_AR_FIRST_REGISTRATION   "audio receiver agent first registration"
#define SZ_AR_MODE                 "audio receiver agent mode"
#define SZ_FEEDBACK_TV_MENU        "feedback tv menu"
#define SZ_FEEDBACK_TV_RETURN      "feedback tv return"

//// rcCmd struct translates ir and tcp input messages to output synthesized 
//// keyboard shortcuts, itach wifi to ir, and zigbee remote control commands.  
//// Translation ouptut is context dependent based on the active application;
//// e.g. live TV, DVD, Windows Media Center recording, Netflix, YouTube, etc.
static struct rcCmd {
   int id;                        // cmdID enum
   const char * sz;               // ip command string input
   struct irRx * irRx;            // ir input
   struct shortcut * shortcut;    // synthesized keyboard shortcut output
   struct irTx * irTx;            // wifi to ir output
   struct cercCmd * zrc;          // zigbee remote control output
} rcCmd[] = { 

   {
      UNDEFINED,
      SZ_UNDEFINED,
      &irRxCmd[TIRA_UNDEFINED].key,
      NULL,
      NULL,
      NULL,
   }, 

   {
      ARROW_DOWN,
      SZ_ARROW_DOWN,
      &irRxCmd[TIRA_VCR_MENU_DOWN].key,
      &virtualKey[SC_ARROW_DOWN].shortcut,
      irTxCmd[ITACH_MENU_DOWN].remote,
      &cercCmd[ZRC_DOWN],
   },
   
   {
      ARROW_LEFT,
      SZ_ARROW_LEFT,
      &irRxCmd[TIRA_VCR_MENU_LEFT].key,
      &virtualKey[SC_LARGE_JUMP_BACKWARD].shortcut,
      irTxCmd[ITACH_MENU_LEFT].remote,
      &cercCmd[ZRC_LEFT],
   },
   
   {
      ARROW_RIGHT,
      SZ_ARROW_RIGHT,
      &irRxCmd[TIRA_VCR_MENU_RIGHT].key,
      &virtualKey[SC_LARGE_JUMP_FORWARD].shortcut,
      irTxCmd[ITACH_MENU_RIGHT].remote,
      &cercCmd[ZRC_RIGHT],
   },
    
   {
      ARROW_UP,
      SZ_ARROW_UP,
      &irRxCmd[TIRA_VCR_MENU_UP].key,
      &virtualKey[SC_ARROW_UP].shortcut,
      irTxCmd[ITACH_MENU_UP].remote,
      &cercCmd[ZRC_UP],
   },
   
   {
      CHANNEL_DOWN,
      SZ_CHANNEL_DOWN,
      &irRxCmd[TIRA_VCR_CHANNEL_DOWN].key,
      NULL,
      irTxCmd[ITACH_CHANNEL_DOWN].remote,
      &cercCmd[ZRC_CHANNEL_DOWN],
   },
   
   {
      CHANNEL_UP,
      SZ_CHANNEL_UP,
      &irRxCmd[TIRA_VCR_CHANNEL_UP].key,
      NULL,
      irTxCmd[ITACH_CHANNEL_UP].remote,
      &cercCmd[ZRC_CHANNEL_UP],
   },
   
   {
      DVD,
      SZ_DVD,
      NULL,
      NULL,
      irTxCmd[ITACH_N2].remote,
      NULL,
   },
   
   {
      DVR,
      SZ_DVR,
      NULL,
      NULL,
      irTxCmd[ITACH_N4].remote,
      NULL,
   },
    
   {
      EJECT,
      SZ_EJECT,
      NULL,
      NULL,
      irTxCmd[ITACH_EJECT].remote,
      NULL,
   },
   
   {
      EXIT_APPLICATION,
      SZ_EXIT_APPLICATION,
      NULL,
      &virtualKey[SC_ALT_F4].shortcut,
      NULL,
      NULL,
   },
   
   {
      FAST_FORWARD,
      SZ_FAST_FORWARD,
      &irRxCmd[TIRA_VCR_FAST_FORWARD].key,
      &virtualKey[SC_SMALL_JUMP_FORWARD].shortcut,
      irTxCmd[ITACH_FAST_FORWARD].remote,
      &cercCmd[ZRC_FAST_FORWARD],
   },
   
   {
      GET_STATE,
      SZ_GET_STATE,
      NULL,
      NULL,
      NULL,
      NULL,
   },
    
   {
      LAUNCH_EXPLORER,
      SZ_LAUNCH_EXPLORER,
      NULL,
      NULL,
      NULL,
      NULL,
   },
   
   {
      REFRESH_EXPLORER,
      SZ_REFRESH_EXPLORER,
      NULL,
      &virtualKey[SC_CTRL_R].shortcut,
      NULL,
      NULL,
   },
    
   {
      MENU,
      SZ_MENU,
      &irRxCmd[TIRA_VCR_MENU].key,
      NULL,
      irTxCmd[ITACH_MENU].remote,
      &cercCmd[ZRC_GUIDE],
   },
    
   {
      N0,
      SZ_0,
      &irRxCmd[TIRA_VCR_N0].key,
      &virtualKey[SC_N0].shortcut,
      irTxCmd[ITACH_N0].remote,
      &cercCmd[ZRC_ZERO],
   },
    
   {
      N1,
      SZ_1,
      &irRxCmd[TIRA_VCR_N1].key,
      &virtualKey[SC_N1].shortcut,
      irTxCmd[ITACH_N1].remote,
      &cercCmd[ZRC_ONE],
   },
    
   {
      N2,
      SZ_2,
      &irRxCmd[TIRA_VCR_N2].key,
      &virtualKey[SC_N2].shortcut,
      irTxCmd[ITACH_N2].remote,
      &cercCmd[ZRC_TWO],
   },
    
   {
      N3,
      SZ_3,
      &irRxCmd[TIRA_VCR_N3].key,
      &virtualKey[SC_N3].shortcut,
      irTxCmd[ITACH_N3].remote,
      &cercCmd[ZRC_THREE],
   },
    
   {
      N4,
      SZ_4,
      &irRxCmd[TIRA_VCR_N4].key,
      &virtualKey[SC_N4].shortcut,
      irTxCmd[ITACH_N4].remote,
      &cercCmd[ZRC_FOUR],
   },
    
   {
      N5,
      SZ_5,
      &irRxCmd[TIRA_VCR_N5].key,
      &virtualKey[SC_N5].shortcut,
      irTxCmd[ITACH_N5].remote,
      &cercCmd[ZRC_FIVE],
   },
    
   {
      N6,
      SZ_6,
      &irRxCmd[TIRA_VCR_N6].key,
      &virtualKey[SC_N6].shortcut,
      irTxCmd[ITACH_N6].remote,
      &cercCmd[ZRC_SIX],
   },
    
   {
      N7,
      SZ_7,
      &irRxCmd[TIRA_VCR_N7].key,
      &virtualKey[SC_N7].shortcut,
      irTxCmd[ITACH_N7].remote,
      &cercCmd[ZRC_SEVEN],
   },
    
   {
      N8,
      SZ_8,
      &irRxCmd[TIRA_VCR_N8].key,
      &virtualKey[SC_N8].shortcut,
      irTxCmd[ITACH_N8].remote,
      &cercCmd[ZRC_EIGHT],
   },
    
   {
      N9,
      SZ_9,
      &irRxCmd[TIRA_VCR_N9].key,
      &virtualKey[SC_N9].shortcut,
      irTxCmd[ITACH_N9].remote,
      &cercCmd[ZRC_NINE],
   },
    
   {
      NEXT_WINDOW,
      SZ_NEXT_WINDOW,
      NULL,
      NULL,
      NULL,
      NULL,
   },
    
   {
      OK,
      SZ_OK,
      &irRxCmd[TIRA_VCR_OK].key,
      &virtualKey[SC_ENTER].shortcut,
      irTxCmd[ITACH_OK].remote,
      &cercCmd[ZRC_OK],
   },
    
   {
      PAUSE,
      SZ_PAUSE,
      &irRxCmd[TIRA_VCR_PAUSE].key,
      &virtualKey[SC_PLAY_PAUSE].shortcut,
      irTxCmd[ITACH_PAUSE].remote,
      &cercCmd[ZRC_PLAY],
   },
    
   {
      PLAY,
      SZ_PLAY,
      &irRxCmd[TIRA_VCR_PLAY].key,
      &virtualKey[SC_PLAY_PAUSE].shortcut,
      irTxCmd[ITACH_PLAY].remote,
      &cercCmd[ZRC_PLAY],
   },
   
   {
      VIDEO_POWER,
      SZ_VIDEO_POWER,
      &irRxCmd[TIRA_VCR_POWER].key,
      NULL,
      irTxCmd[ITACH_POWER].remote,
      NULL,
   },
   
   {
      RESTORE_STATE,
      SZ_RESTORE_STATE,
      NULL,
      NULL,
      NULL,
      NULL,
   },
   
   {
      RETURN,
      SZ_RETURN,
      NULL,
      NULL,
      irTxCmd[ITACH_RETURN].remote,
      NULL,
   },
   
   {
      REWIND,
      SZ_REWIND,
      &irRxCmd[TIRA_VCR_REWIND].key,
      &virtualKey[SC_SMALL_JUMP_BACKWARD].shortcut,
      irTxCmd[ITACH_REWIND].remote,
      &cercCmd[ZRC_REWIND],
   },
   
   {
      STOP,
      SZ_STOP,
      &irRxCmd[TIRA_VCR_STOP].key,
      &virtualKey[SC_ALT_F4].shortcut,
      irTxCmd[ITACH_STOP].remote,
      &cercCmd[ZRC_EXIT_PAGE],
   },
   
   {
      TV,
      SZ_TV,
      NULL,
      NULL,
      irTxCmd[ITACH_N4].remote,
      NULL,
   },
    
   {
      STATUS,
      SZ_STATUS,
      NULL,
      NULL,
      irTxCmd[ITACH_DISPLAY].remote,
      NULL,
   },
   
   {
      SURROUND,
      SZ_SURROUND,
      NULL,
      NULL,
      NULL,
      NULL,
   },
   
   {
      MUTE,
      SZ_MUTE,
      &irRxCmd[TIRA_MUTE].key,
      NULL,
      NULL,
      NULL,
   },
    
   {
      VIDEO_INPUT_MENU,
      SZ_MODE,
      &irRxCmd[TIRA_VCR_VIDEO].key,
      NULL,
      irTxCmd[ITACH_VIDEO].remote,
      NULL,
   },
    
   {
      VOLUME_DOWN,
      SZ_VOLUME_DOWN,
      &irRxCmd[TIRA_VOLUME_DOWN].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      VOLUME_UP,
      SZ_VOLUME_UP,
      &irRxCmd[TIRA_VOLUME_UP].key,
      NULL,
      NULL,
      NULL,
   },
   
// begin existing equipment and itach notifications   
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_CHAPTER_BACK].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_CHAPTER_NEXT].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_DISPLAY].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_EJECT].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_FAST_FORWARD].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_MENU].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_MENU_DOWN].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_MENU_LEFT].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_MENU_RIGHT].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_MENU_UP].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N0].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N1].key,
      NULL,
      NULL,
      NULL,
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N2].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N3].key,
      NULL,
      NULL,
      NULL,
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N4].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N5].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N6].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N7].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N8].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_N9].key,
      NULL,
      NULL,
      NULL,
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_OK].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_PAUSE].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_PLAY].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_POWER].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_REWIND].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_DVD_STOP].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_HDMI_SOURCE_INPUT1].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_HDMI_SOURCE_INPUT2].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_HDMI_SOURCE_INPUT3].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_HDMI_SOURCE_TOGGLE].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_CHANNEL_DOWN].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_CHANNEL_UP].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_DISPLAY].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_FAST_FORWARD].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_MENU].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_MENU_DOWN].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_MENU_LEFT].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_MENU_RIGHT].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_MENU_UP].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N0].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N1].key,
      NULL,
      NULL,
      NULL,
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N2].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N3].key,
      NULL,
      NULL,
      NULL,
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N4].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N5].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N6].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N7].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N8].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_N9].key,
      NULL,
      NULL,
      NULL,
   },

   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_PAUSE].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_PIP].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_PLAY].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_POWER].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_RETURN].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_REWIND].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_STOP].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      IR_IGNORE,
      SZ_IR_IGNORE,
      &irRxCmd[TIRA_TV_VIDEO].key,
      NULL,
      NULL,
      NULL,
   },
   
   {
      INFO,
      SZ_DISPLAY_INFO,
      NULL,
      NULL,
      NULL,
      &cercCmd[ZRC_DISPLAY_INFO],
   },
   
   {
      EXIT_PAGE,
      SZ_EXIT_PAGE,
      NULL,
      NULL,
      NULL,
      &cercCmd[ZRC_EXIT_PAGE],
   },
   
   {
      GUIDE,
      SZ_GUIDE,
      NULL,
      NULL,
      NULL,
      &cercCmd[ZRC_GUIDE],
   },
   
   {
      RECALL,
      SZ_PREVIOUS_CHANNEL,
      NULL,
      NULL,
      NULL,
      &cercCmd[ZRC_PREVIOUS_CHANNEL],
   },
   
   {
      PAGE_UP,
      SZ_PAGE_UP,
      NULL,
      NULL,
      NULL,
      &cercCmd[ZRC_PAGE_UP],
   },
   
   {
      PAGE_DOWN,
      SZ_PAGE_DOWN,
      NULL,
      NULL,
      NULL,
      &cercCmd[ZRC_PAGE_DOWN],
   },
   
   {
      NEXT_FAVORITE,
      SZ_NEXT_FAVORITE,
      NULL,
      NULL,
      NULL,
      &cercCmd[ZRC_NEXT_FAVORITE],
   },
   
   {
      CHANNEL,
      SZ_CHANNEL,
      NULL,
      NULL,
      NULL,
      NULL,
   },   

// internal commands   
   
   {
      AR_FIRST_REGISTRATION,
      SZ_AR_FIRST_REGISTRATION,
      NULL,
      NULL,
      NULL,
      NULL,
   },
   
   {
      AR_MODE,
      SZ_AR_MODE,
      NULL,
      NULL,
      NULL,
      NULL,
   },
   
   {
      FEEDBACK_TV_MENU,
      SZ_FEEDBACK_TV_MENU,
      NULL,
      NULL,
      NULL,
      NULL,
   },

   {
      FEEDBACK_TV_RETURN,
      SZ_FEEDBACK_TV_RETURN,
      NULL,
      NULL,
      NULL,
      NULL,
   },
   
// general commands   
   
   {
      EXIT_PROCESS,
      SZ_EXIT_PROCESS,
      NULL,
      NULL,
      NULL,
      NULL,
   }
};

// enumerate whether an itach command depends on the success of the previous itach command
enum itachDependency {
   INDEPENDENT,
   DEPENDENT
};   

// global itachCmd instance used to process itach commands 
class ItachCmd {
   public:
      struct irTx * irTx;          // itach command structure
      struct irRx * irRx;          // tira irRx structure expected to confirm a successful itach command request
      struct irRx * irRxPending;   // expected IR command from the itach
      BOOL irComplete;             // TRUE when irComplete message received from itach
      BOOL prerequisiteFailure;    // TRUE when INDEPENDENT && !(irComplete && !irRxPending)
      char debug[1024];            // tira irRxCmdCallback debug info from itach
      int delay;                   // millisecond processing delay allowance
      int dependent;               // DEPENDENT if this command depends on success of previous command 
      
   ItachCmd() {
      irTx=NULL;
      irRx=NULL;
      irRxPending=NULL;
      irComplete=TRUE;
      prerequisiteFailure=FALSE;
      debug[0]='\0';
      delay=0;
      dependent=INDEPENDENT;
   }
      
   void set(
      struct irTx * irTx,
      int dependent,
      int delay) {
      this->irTx = irTx;
      this->irRx = irTxSzToIrRx(irTx->sz);
      //printf("itachCmdArg: irRx %s\n", irRx->sz);
      this->dependent = dependent;
      this->delay = delay;
   }
   
   private:
   // return the tira irRx structure that corresonds to an itach irTx
   // command.  The irRx struct is used to confirm a requested itach 
   // transmit is successfully received.
   struct irRx * irTxSzToIrRx(const char * sz) {
      int i;
      const char * match;
      
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
} * itachCmd;


static struct socketDescriptor arAgentSd = {
   (char *) AUDIO_RECEIVER_AGENT_HOSTNAME,
   AUDIO_CONTROL_PORT,
   INVALID_SOCKET,
   NULL,
   NO_ERROR,
   FALSE,
   FALSE,
   FALSE,
   "arAgent"
};

static struct socketDescriptor itachSd = {
   (char *) ITACH_AGENT_HOSTNAME,
   ITACH_CMD_PORT,
   INVALID_SOCKET,
   NULL,
   NO_ERROR,
   FALSE,
   FALSE,
   FALSE,
   "itach"
};

const char * tira_dll_absolute_path = "c:\\utils\\startup\\local\\rcagent\\tira2.dll";

BOOL forceForegroundWindow(HWND);
BOOL isAudioFromVideo(void);
BOOL isCmdStateSet(int);
BOOL isExplorerForeground(void);
BOOL isVideoPlayerActive(void);
char * getForegroundWindowImageFileName(char *);
const char * remoteModeToSz(int);
const char * vkCodeToSz(unsigned);
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
struct rcCmd * irRxCmdToRcCmd(char const *);
void errMsgExit(char *);
void initZigbeeRemoteControl(void);
void launchExplorer(const char *);
void processArAgentNotification(char *);
void processClientCmd(QueuedMsg *);
void processItachCmd(void);
void restoreReceiverState(void);
void restoreReceiverState(void);
void sendArCmd(const char *);
void sendDefault(struct rcCmd *);
void sendItachCmd(int, int);
void sendItachCmd(int, int, int, int);
void sendStateInformation(QueuedMsg *);
void sendVirtualKeyCode(unsigned);
void sendVirtualKeyCode(unsigned, int);
void sendZigbeeChannelRequest(const char *);
void setAudioMode(int);
void setAudioSource(int);
void setDvdPower(int);
void setNextForegroundWindow(void);
void setSpeakerVolume(const char *, const char *);
void setSurroundSound(int);
void setVideoMode(int);
void setVideoSource(int);
void toggleVideoPower(void);
void videoModeChangeThreadStart(BOOL, int);
static char szDefaultMasterVolume[] = SZ_MASTER_VOLUME SZ_VIDEO_DEFAULT_MASTER_VOLUME;
static DWORD irRxTime=0, queuedIrRxTime=0;
static int displayMode=UNDEFINED, nextMode=UNDEFINED;
static int tvPowered=FALSE, dvdPowered=FALSE;
static int dependentTimeout=FALSE;
static int registered=FALSE;


