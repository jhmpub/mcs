//// 
//// virtual key codes to synthesize shortcut key input
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
   { "YouTube - ", YOUTUBE },
   { "youtube.com", YOUTUBE }
}; 

enum vkCodeId {
   SC_LARGE_JUMP_BACKWARD,
   SC_LARGE_JUMP_FORWARD,
   SC_SMALL_JUMP_BACKWARD,
   SC_SMALL_JUMP_FORWARD,
   SC_PLAY_PAUSE,
   SC_ALT_TAB,
   SC_ARROW_UP,
   SC_ARROW_DOWN,
   SC_ENTER,
   SC_ALT_F4,
   SC_CTRL_R,
   SC_N0,
   SC_N1,
   SC_N2,
   SC_N3,
   SC_N4,
   SC_N5,
   SC_N6,
   SC_N7,
   SC_N8,
   SC_N9,
   SHORTCUTS
};   
   
struct shortcut {
   unsigned vkCode[APP_SIZE];    // indexed by appID enum
   const char * sz[APP_SIZE];    // debug description
};   

static struct virtualKey {
   int id;
   struct shortcut shortcut;
} virtualKey[SHORTCUTS] = {
   {
      SC_LARGE_JUMP_BACKWARD,
      { 
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
      }  
   },
   {
      SC_LARGE_JUMP_FORWARD,
      {
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
      }
   },   
   { 
      SC_SMALL_JUMP_BACKWARD,
      {
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
      }
   },   
   { 
      SC_SMALL_JUMP_FORWARD,
      {
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
      }
   },   
   {
      SC_PLAY_PAUSE,
      {
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
      }
   },
   {
      SC_ALT_TAB,
      {
         {KBD_ALT_TAB,0,0,0}, 
         {SZ_KBD_ALT_TAB,NULL,NULL,NULL}
      }   
   },
   {
      SC_ARROW_UP,
      {
         {KBD_ARROW_UP,0,0,0}, 
         {SZ_KBD_ARROW_UP,NULL,NULL,NULL}
      }
   },
   {
      SC_ARROW_DOWN,
      {
         {KBD_ARROW_DOWN,0,0,0}, 
         {SZ_KBD_ARROW_DOWN,NULL,NULL,NULL}
      }   
   },
   {
      SC_ENTER,
      {
         {KBD_ENTER,0,0,0}, 
         {SZ_KBD_ENTER,NULL,NULL,NULL}
      }   
   },
   {
      SC_ALT_F4,
      {
         {KBD_ALT_F4,0,0,0}, 
         {SZ_KBD_ALT_F4,NULL,NULL,NULL}
      }
   },
   {
      SC_CTRL_R,
      {
         {KBD_CTRL_R,0,0,0}, 
         {SZ_KBD_CTRL_R,NULL,NULL,NULL}
      }   
   },
   {
      SC_N0,
      {
         {KBD_N0,0,0,0}, 
         {SZ_KBD_N0,NULL,NULL,NULL}
      }   
   },
   {
      SC_N1,
      {
         {KBD_N1,0,0,0}, 
         {SZ_KBD_N1,NULL,NULL,NULL}
      }   
   },
   {
      SC_N2,
      {
         {KBD_N2,0,0,0}, 
         {SZ_KBD_N2,NULL,NULL,NULL}
      }   
   },
   {
      SC_N3,
      {
         {KBD_N3,0,0,0}, 
         {SZ_KBD_N3,NULL,NULL,NULL}
      }   
   },
   {
      SC_N4,
      {
         {KBD_N4,0,0,0}, 
         {SZ_KBD_N4,NULL,NULL,NULL}
      }   
   },
   {
      SC_N5,
      {
         {KBD_N5,0,0,0}, 
         {SZ_KBD_N5,NULL,NULL,NULL}
      }   
   },
   {
      SC_N6,
      {
         {KBD_N6,0,0,0}, 
         {SZ_KBD_N6,NULL,NULL,NULL}
      }   
   },
   {
      SC_N7,
      {
         {KBD_N7,0,0,0}, 
         {SZ_KBD_N7,NULL,NULL,NULL}
      }   
   },
   {
      SC_N8,
      {
         {KBD_N8,0,0,0}, 
         {SZ_KBD_N8,NULL,NULL,NULL}
      }   
   },
   {
      SC_N9,
      {
         {KBD_N9,0,0,0}, 
         {SZ_KBD_N9,NULL,NULL,NULL}
      }   
   }
};   

