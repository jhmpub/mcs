#define PRINTERROR(s)	\
		printf("\n%s error: %d\n", s, WSAGetLastError());
        
#define  OFF 0
#define  ON  1        

// virtual key codes to synthesize pressing keyboard shortcuts
// generate these using the mcs/util/vkcode.exe utility
#define  EXIT_APPLICATION          0x7312   // remote Stop -> Alt-F4
#define  REFRESH_EXPLORER          0x5211   // prepends remote Up, Down, VCR Menu, TV PIP
#define  VLC_CHAPTER_NEXT          0x4E10   // remote Right Menu Arrow -> Shift-N
#define  VLC_CHAPTER_PREVIOUS      0x5010   // remote Left Menu Arrow -> Shift-P
#define  VLC_JUMP_BACKWARD         0x2512   // remote << -> Alt-Left Arrow
#define  VLC_JUMP_FORWARD          0x2712   // remote >> -> Alt-Right Arrow
#define  VLC_PLAY_PAUSE            0x0020   // remote " -> Space
#define  WMC_JUMP_BACKWARD         0x4211   // remote << -> Ctrl-B        
#define  WMC_JUMP_FORWARD          0x4611   // remote >> -> Ctrl-F         
#define  WMC_PLAY_PAUSE            0x5011   // remote " -> Ctrl-P
#define  KBD_ARROW_UP              0x0026   // remote Up Menu Arrow -> Up Keyboard Arrow
#define  KBD_ARROW_DOWN            0x0028   // remote Down Menu Arrow -> Down Keyboard Arrow
#define  KBD_ENTER                 0x000D   // remote > -> Enter
#define  KBD_ALT_TAB               0x0912   // remote VCR Menu, TV PIP -> Alt-Tab

#define REMOTE_MUTE                 0
#define REMOTE_TV_JUMP_BACKWARD     1
#define REMOTE_TV_JUMP_FORWARD      2
#define REMOTE_TV_MENU_DOWN         3
#define REMOTE_TV_MENU_UP           4
#define REMOTE_TV_MENU_RIGHT        5
#define REMOTE_TV_MENU_LEFT         6
#define REMOTE_TV_PIP               7
#define REMOTE_TV_PAUSE             8
#define REMOTE_TV_POWER_TOGGLE      9
#define REMOTE_TV_STOP             10
#define REMOTE_TV_PLAY             11
#define REMOTE_TV_VIDEO            12
#define REMOTE_VCR_JUMP_BACKWARD   13
#define REMOTE_VCR_JUMP_FORWARD    14
#define REMOTE_VCR_MENU            15
#define REMOTE_VCR_MENU_DOWN       16 
#define REMOTE_VCR_MENU_LEFT       17
#define REMOTE_VCR_MENU_RIGHT      18
#define REMOTE_VCR_MENU_UP         19
#define REMOTE_VCR_PAUSE           20
#define REMOTE_VCR_STOP            21 
#define REMOTE_VCR_PLAY            22
#define REMOTE_VOLUME_DOWN         24
#define REMOTE_VOLUME_UP           25
#define REMOTE_UNDEFINED          100

// cmd codes are 12 chars but matching the first 4 is unique and aids matching
#define CMD_CODE_LENGTH            4

#define SZ_REMOTE_MUTE              "remote mute"
#define SZ_REMOTE_TV_JUMP_BACKWARD  "remote tv backward jump"
#define SZ_REMOTE_TV_JUMP_FORWARD   "remote tv forward jump"
#define SZ_REMOTE_TV_MENU_RIGHT     "remote tv menu right"
#define SZ_REMOTE_TV_MENU_LEFT      "remote tv menu left"
#define SZ_REMOTE_TV_MENU_DOWN      "remote tv menu down"
#define SZ_REMOTE_TV_MENU_UP        "remote tv menu up"
#define SZ_REMOTE_TV_PIP            "remote tv pip"
#define SZ_REMOTE_TV_PAUSE          "remote tv pause"
#define SZ_REMOTE_TV_PLAY           "remote tv play"
#define SZ_REMOTE_TV_POWER_TOGGLE   "remote tv power toggle"
#define SZ_REMOTE_TV_STOP           "remote tv stop"
#define SZ_REMOTE_TV_VIDEO          "remote tv/video source selector"
#define SZ_REMOTE_VCR_JUMP_BACKWARD "remote vcr backward jump"
#define SZ_REMOTE_VCR_JUMP_FORWARD  "remote vcr forward jump"
#define SZ_REMOTE_VCR_MENU          "remote vcr menu"     
#define SZ_REMOTE_VCR_MENU_DOWN     "remote vcr menu down"
#define SZ_REMOTE_VCR_MENU_LEFT     "remote vcr menu left"
#define SZ_REMOTE_VCR_MENU_RIGHT    "remote vcr menu right"
#define SZ_REMOTE_VCR_MENU_UP       "remote vcr menu up"
#define SZ_REMOTE_VCR_PAUSE         "remote vcr pause"
#define SZ_REMOTE_VCR_PLAY          "remote vcr play"
#define SZ_REMOTE_VCR_STOP          "remote vcr stop"
#define SZ_REMOTE_VCR_OK            "remote vcr ok"
#define SZ_REMOTE_VOLUME_DOWN       "remote volume down"
#define SZ_REMOTE_VOLUME_UP         "remote volume up"
#define SZ_REMOTE_UNDEFINED         "remote undefined"


char * usage = "\nUsage: rcClient [-port <port>]\n";
char tira_dll_absolute_path[] = "c:\\utils\\startup\\local\\rcclient\\tira2.dll";

#ifdef CONSOLE_MODE
#include <termios.h>
static struct termios old_tio, new_tio;
#endif

// Tira encoded IR messages received from a universal remote
const char * irRemoteMute[] = {
"E06180610003",
"A22080200003",
"A62080200003",
"61E380E300BC",
"E22080600003",
"A62180200003",
"E22080600003",
"A02180200003",
"A02080200003",
"A260C0200003",
"A22180200003",
"E620C0200003",
"A320C0600003",
"A660C0200003",
"E02080200003",
"E021C0210003",
"A660C0200003",
"E260C0200003",
"A06080600003",
"E32080200003",
"A32180200003",
"A26180200003"
};

const char * irRemoteTvBackwardJump[] = {
"2A8808880003",
"2A8809880003",
"2A8818880003",
"2E8808880003",
"6A8808880003",
"288808880003",
};

const char * irRemoteTvForwardJump[] = {
"2A0808080003",
"280808080003",
"2A1808080003",
"2E0808080003",
"2A0838080003",
"2A0808180003",
"7A1818180003",
"6A0808080003",
"6A1808080003",
"E26180610003",
};

const char * irRemoteTvMenuRight[] = {
"0AAA28AA0003"
};

const char * irRemoteTvMenuLeft[] = {
"0A2A282A0003"
};

const char * irRemoteTvMenuDown[] = {
"0AA228A20003"
};

const char * irRemoteTvMenuUp[] = {
"0A2228220003"
};

const char * irRemoteTvPIP[] = {
"8A8AA88A0003",
"8A8BA88A0003",
"8E8AA88A0003"
};

const char * irRemoteTvPause[] = {
"8A0AA80A0003",
"8A1AA80A0003",
"880AA80A0003",
"9A0AA80A0003",
"8A0BA80A0003"
};

const char * irRemoteTvPlay[] = {
"8A2AA82A0003",
"8A2AB82A0003"
};

const char * irRemoteTvPowerToggle[] = {
"A09B809E0003",
"A29A808A0003",
"A28E808A0003",
"E09F809F0103",
"E29BC19F0103",
"E68F809A0103",
"A68AC08A0003",
"E28BC08A0103",
"E28A818E0003",
"A08A808A0003",
"A38AA3220081",
"61B583B503BC",
"A28A808A0003",
"A08B808B0003",
"A38E818A0003",
"A28B808A0003",
"A09A808E0003",
"A69A808A0003",
"E68A808A0003",
"E08E808A0003",
"A68BC08A0103",
"E68E808A0003",
"A29E808E0003",
"E08AC08A0003",
"A68E808A0003",
"A28A808A0003",
"A08E808A0003",
"E69AC08A0003",
"E28F808E0103",
"A29B808A0103",
"E29A808A0003",
"A38B808A0003",
"A28F809A0003",
"035555ED0100", // cbl/sat mode
"231700000000"  // vcr mode
};

const char * irRemoteTvStop[] = {
"2AA808A80003",
"2EA808A80003",
"6AA808A80003",
"2AE808A80003",
"2AB808A80003",
"2AA838A80003"
};

const char * irRemoteTvVideo[] = {
"028820880003",
"029821880003",
"068820880003",
"008821880003",
"015800000000",   // vcr mode
"105355C80300",   // cbl/sat mode
};

const char * irRemoteVcrPause[] = {
"721600000000"
};

const char * irRemoteVcrPlay[] = {
"1E1600000000"
};

const char * irRemoteVcrBackwardJump[] = {
"131600000000"
};

const char * irRemoteVcrForwardJump[] = {
"1C1600000000"
};

const char * irRemoteVcrMenu[] = {
"0C0000000000",
"0C5000000000"
};

const char * irRemoteVcrMenuRight[] = {
"060000000000",
"065000000000"
};

const char * irRemoteVcrMenuLeft[] = {
"015000000000",
"010000000000"
};

const char * irRemoteVcrMenuUp[] = {
"3F3001000000",
"3F0000000000"
};

const char * irRemoteVcrMenuDown[] = {
"3D3001000000",
"3D0000000000"
};

const char * irRemoteVcrStop[] = {
"191600000000"
};

const char * irRemoteVcrOK[] = {
"1F3001000000",
"1F0000000000"
};

const char * irRemoteVolumeDown[] = {
"228000800003",
"E180038003BC",
"608001800103",
"628000800003",
"668031800003",
"208000800003",
"638000800003",
"268000800003",
"238000800003",
};

const char * irRemoteVolumeUp[] = {
"220000000003",
};

        
static struct translation {
    char * description;
    unsigned id;
    unsigned key;    // virtual key code used to synthesize a shortcut key
    unsigned size;   // number of cmdCode strings
    char const * const * const cmdCodes;
} translation[] = { 

    {SZ_UNDEFINED,
     UNDEFINED,
     0,
     0,
     NULL}, 

    {SZ_REMOTE_MUTE,
     REMOTE_MUTE, 
     0,
     sizeof(irRemoteMute)/sizeof(char *),
     irRemoteMute},
     
    {SZ_REMOTE_TV_JUMP_BACKWARD,
     REMOTE_TV_JUMP_BACKWARD,
     WMC_JUMP_BACKWARD,
     sizeof(irRemoteTvBackwardJump)/sizeof(char *),
     irRemoteTvBackwardJump},
     
    {SZ_REMOTE_TV_JUMP_FORWARD,
     REMOTE_TV_JUMP_FORWARD,
     WMC_JUMP_FORWARD,
     sizeof(irRemoteTvForwardJump)/sizeof(char *),
     irRemoteTvForwardJump},
     
    {SZ_REMOTE_TV_MENU_RIGHT,
     REMOTE_TV_MENU_RIGHT,
     0,
     sizeof(irRemoteTvMenuRight)/sizeof(char *),
     irRemoteTvMenuRight},
     
    {SZ_REMOTE_TV_MENU_LEFT,
     REMOTE_TV_MENU_LEFT,
     0,
     sizeof(irRemoteTvMenuLeft)/sizeof(char *),
     irRemoteTvMenuLeft},
     
    {SZ_REMOTE_TV_MENU_DOWN,
     REMOTE_TV_MENU_DOWN,
     KBD_ARROW_DOWN,
     sizeof(irRemoteTvMenuDown)/sizeof(char *),
     irRemoteTvMenuDown},
     
    {SZ_REMOTE_TV_MENU_UP,
     REMOTE_TV_MENU_UP,
     KBD_ARROW_UP,
     sizeof(irRemoteTvMenuUp)/sizeof(char *),
     irRemoteTvMenuUp},

    {SZ_REMOTE_TV_PIP,
     REMOTE_TV_PIP,
     KBD_ALT_TAB,
     sizeof(irRemoteTvPIP)/sizeof(char *),
     irRemoteTvPIP},
     
    {SZ_REMOTE_TV_PAUSE,
     REMOTE_TV_PAUSE,
     WMC_PLAY_PAUSE,
     sizeof(irRemoteTvPause)/sizeof(char *),
     irRemoteTvPause},

    {SZ_REMOTE_TV_PLAY,
     REMOTE_TV_PLAY,
     KBD_ENTER,
     sizeof(irRemoteTvPlay)/sizeof(char *),
     irRemoteTvPlay},

    {SZ_REMOTE_TV_POWER_TOGGLE,
     REMOTE_TV_POWER_TOGGLE,
     0,
     sizeof(irRemoteTvPowerToggle)/sizeof(char *),
     irRemoteTvPowerToggle},
     
    {SZ_REMOTE_TV_STOP,
     REMOTE_TV_STOP,
     EXIT_APPLICATION,
     sizeof(irRemoteTvStop)/sizeof(char *),
     irRemoteTvStop},
     
    {SZ_REMOTE_TV_VIDEO,
     REMOTE_TV_VIDEO,
     0,
     sizeof(irRemoteTvVideo)/sizeof(char *),
     irRemoteTvVideo},

    {SZ_REMOTE_VCR_JUMP_BACKWARD,
     REMOTE_VCR_JUMP_BACKWARD,
     VLC_JUMP_BACKWARD,
     sizeof(irRemoteVcrBackwardJump)/sizeof(char *),
     irRemoteVcrBackwardJump},
     
    {SZ_REMOTE_VCR_JUMP_FORWARD,
     REMOTE_VCR_JUMP_FORWARD,
     VLC_JUMP_FORWARD,
     sizeof(irRemoteVcrForwardJump)/sizeof(char *),
     irRemoteVcrForwardJump},
     
    {SZ_REMOTE_VCR_MENU,
     REMOTE_VCR_MENU,
     KBD_ALT_TAB,
     sizeof(irRemoteVcrMenu)/sizeof(char *),
     irRemoteVcrMenu},
     
    {SZ_REMOTE_VCR_MENU_DOWN,
     REMOTE_VCR_MENU_DOWN,
     KBD_ARROW_DOWN,
     sizeof(irRemoteVcrMenuDown)/sizeof(char *),
     irRemoteVcrMenuDown},
     
    {SZ_REMOTE_VCR_MENU_LEFT,
     REMOTE_VCR_MENU_LEFT,
     VLC_CHAPTER_PREVIOUS,
     sizeof(irRemoteVcrMenuLeft)/sizeof(char *),
     irRemoteVcrMenuLeft},
     
    {SZ_REMOTE_VCR_MENU_RIGHT,
     REMOTE_VCR_MENU_RIGHT,
     VLC_CHAPTER_NEXT,
     sizeof(irRemoteVcrMenuRight)/sizeof(char *),
     irRemoteVcrMenuRight},
     
    {SZ_REMOTE_VCR_MENU_UP,
     REMOTE_VCR_MENU_UP,
     KBD_ARROW_UP,
     sizeof(irRemoteVcrMenuUp)/sizeof(char *),
     irRemoteVcrMenuUp},
     
    {SZ_REMOTE_VCR_PAUSE,
     REMOTE_VCR_PAUSE,
     VLC_PLAY_PAUSE,
     sizeof(irRemoteVcrPause)/sizeof(char *),
     irRemoteVcrPause},
     
    {SZ_REMOTE_VCR_PLAY,
     REMOTE_VCR_PLAY,
     KBD_ENTER,
     sizeof(irRemoteVcrPlay)/sizeof(char *),
     irRemoteVcrPlay},

    {SZ_REMOTE_VCR_STOP,
     REMOTE_VCR_STOP,
     EXIT_APPLICATION,
     sizeof(irRemoteVcrStop)/sizeof(char *),
     irRemoteVcrStop},
     
    {SZ_REMOTE_VOLUME_DOWN,
     REMOTE_VOLUME_DOWN,
     0,
     sizeof(irRemoteVolumeDown)/sizeof(char *),
     irRemoteVolumeDown},
     
    {SZ_REMOTE_VOLUME_UP,
     REMOTE_VOLUME_UP,
     0,
     sizeof(irRemoteVolumeUp)/sizeof(char *),
     irRemoteVolumeUp}
};

static struct windowList {
    char title[1024];
    HWND hWnd;
    unsigned count;  // number of windows matching the given title
} windowList[256];

static char * dirList[] = {
   "I:\\Recorded TV",
   "I:\\home\\jhm\\video\\lnk"
};
   
char isConsoleMode(void);
char * cmdIdToSz(int);
char * processArgs(int, char **);
char * getHostname(void);
DWORD getTickCountPlus(DWORD);
DWORD rxBroadcastThread(LPVOID);
int cmdSzToId(char *);
int __stdcall irCmdCallback(char const *);
int filterCmd(int);
struct translation * lookup(char const *);
struct irCmd * cmdIdToIrCmd(int);
void setNextForegroundWindow(void);
void dispatchThread(void);
void errMsgExit(char *);
void help(void);
void initDefaults(void);
void initRxBroadcastThread(void);
void initWinsock(void);
void loadTiraDLL(void);
void printErrorAndExit(char *);
void sendCmd(char *);
void setAudioSource(int);
void setSurroundSound(int);
void sendVirtualKeyCode(unsigned);
void stty_buffered(void);
void stty_unbuffered(void);
void ttyLoop(void);
void updateState(int);
HANDLE initExitHandler(void);
LRESULT WINAPI exitHandler(HWND, UINT, WPARAM, LPARAM);
static char szDefaultMasterVolume[sizeof(SZ_MASTER_VOLUME) + 
                                  sizeof(SZ_DEFAULT_MASTER_VOLUME) + 1];
static int muteState, displayState, tvPowered, audioSource;
static SOCKET rxSocket, txSocket;
static SOCKADDR_IN acServer; 

