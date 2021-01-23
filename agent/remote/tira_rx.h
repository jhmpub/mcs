////
//// Tira encoded IR messages received from a universal remote
////

// cmd codes are 12 chars but matching the first 4 is unique enough to aid matching
#define CMD_CODE_LENGTH          4
// extended cmd codes require matching the first 6 chars for uniqueness
#define CMD_CODE_LENGTH_EXT      6

// minimum time for user to stop pressing a key to throw away duplicates
#define IR_RX_QUIET_PERIOD 500   

// minimum time for user to press a different key
// believe this is to be due to ambient noise from plasma tv ir radiation
// noise is not a problem when the plasma tv display is off
#define IR_RX_NOISE_PERIOD 1000   

#define SZ_IR_DVD_CHAPTER_BACK   "ir dvd chapter back"
#define SZ_IR_DVD_CHAPTER_NEXT   "ir dvd chapter next"
#define SZ_IR_DVD_INFO           "ir dvd info"   // sony remote n/a
#define SZ_IR_DVD_EJECT          "ir dvd eject"  // sony remote n/a
#define SZ_IR_DVD_FAST_FORWARD   "ir dvd fast forward"
#define SZ_IR_DVD_MENU           "ir dvd menu"
#define SZ_IR_DVD_MENU_DOWN      "ir dvd menu down"
#define SZ_IR_DVD_MENU_LEFT      "ir dvd menu left"
#define SZ_IR_DVD_MENU_RIGHT     "ir dvd menu right"
#define SZ_IR_DVD_MENU_UP        "ir dvd menu up"
#define SZ_IR_DVD_N0             "ir dvd n0"
#define SZ_IR_DVD_N1             "ir dvd n1"
#define SZ_IR_DVD_N2             "ir dvd n2"
#define SZ_IR_DVD_N3             "ir dvd n3"
#define SZ_IR_DVD_N4             "ir dvd n4"
#define SZ_IR_DVD_N5             "ir dvd n5"
#define SZ_IR_DVD_N6             "ir dvd n6"
#define SZ_IR_DVD_N7             "ir dvd n7"
#define SZ_IR_DVD_N8             "ir dvd n8"
#define SZ_IR_DVD_N9             "ir dvd n9"
#define SZ_IR_DVD_OK             "ir dvd ok"
#define SZ_IR_DVD_EJECT          "ir dvd eject"
#define SZ_IR_DVD_PAUSE          "ir dvd pause"
#define SZ_IR_DVD_PLAY           "ir dvd play"
#define SZ_IR_DVD_POWER          "ir dvd power"
#define SZ_IR_DVD_REWIND         "ir dvd rewind"
#define SZ_IR_DVD_STOP           "ir dvd stop"
#define SZ_IR_TV_CHANNEL_DOWN    "ir tv channel down"
#define SZ_IR_TV_CHANNEL_UP      "ir tv channel up"
#define SZ_IR_TV_DISPLAY         "ir tv display"
#define SZ_IR_TV_FAST_FORWARD    "ir tv fast forward"
#define SZ_IR_TV_MENU            "ir tv menu"
#define SZ_IR_TV_MENU_DOWN       "ir tv menu down"
#define SZ_IR_TV_MENU_LEFT       "ir tv menu left"
#define SZ_IR_TV_MENU_RIGHT      "ir tv menu right"
#define SZ_IR_TV_MENU_UP         "ir tv menu up"
#define SZ_IR_TV_PAUSE           "ir tv pause"
#define SZ_IR_TV_PIP             "ir tv pip"
#define SZ_IR_TV_PLAY            "ir tv play"
#define SZ_IR_TV_POWER           "ir tv power"
#define SZ_IR_TV_RETURN          "ir tv return"
#define SZ_IR_TV_REWIND          "ir tv rewind"
#define SZ_IR_TV_STOP            "ir tv stop"
#define SZ_IR_TV_N0              "ir tv n0"
#define SZ_IR_TV_N1              "ir tv n1"
#define SZ_IR_TV_N2              "ir tv n2"
#define SZ_IR_TV_N3              "ir tv n3"
#define SZ_IR_TV_N4              "ir tv n4"
#define SZ_IR_TV_N5              "ir tv n5"
#define SZ_IR_TV_N6              "ir tv n6"
#define SZ_IR_TV_N7              "ir tv n7"
#define SZ_IR_TV_N8              "ir tv n8"
#define SZ_IR_TV_N9              "ir tv n9"
#define SZ_IR_TV_VIDEO           "ir tv video"
#define SZ_IR_VCR_CHANNEL_DOWN   "ir vcr channel down"
#define SZ_IR_VCR_CHANNEL_UP     "ir vcr channel up"
#define SZ_IR_VCR_FAST_FORWARD   "ir vcr fast forward"
#define SZ_IR_VCR_MENU           "ir vcr menu"     
#define SZ_IR_VCR_MENU_DOWN      "ir vcr menu down"
#define SZ_IR_VCR_MENU_LEFT      "ir vcr menu left"
#define SZ_IR_VCR_MENU_RIGHT     "ir vcr menu right"
#define SZ_IR_VCR_MENU_UP        "ir vcr menu up"
#define SZ_IR_VCR_OK             "ir vcr ok"
#define SZ_IR_VCR_PAUSE          "ir vcr pause"
#define SZ_IR_VCR_PLAY           "ir vcr play"
#define SZ_IR_VCR_POWER          "ir vcr power"
#define SZ_IR_VCR_REWIND         "ir vcr rewind"
#define SZ_IR_VCR_STOP           "ir vcr stop"
#define SZ_IR_VCR_N0             "ir vcr n0"
#define SZ_IR_VCR_N1             "ir vcr n1"
#define SZ_IR_VCR_N2             "ir vcr n2"
#define SZ_IR_VCR_N3             "ir vcr n3"
#define SZ_IR_VCR_N4             "ir vcr n4"
#define SZ_IR_VCR_N5             "ir vcr n5"
#define SZ_IR_VCR_N6             "ir vcr n6"
#define SZ_IR_VCR_N7             "ir vcr n7"
#define SZ_IR_VCR_N8             "ir vcr n8"
#define SZ_IR_VCR_N9             "ir vcr n9"
#define SZ_IR_VCR_VIDEO          "ir vcr video"
#define SZ_IR_HDMI_SOURCE_INPUT1 "ir hdmi source input 1"
#define SZ_IR_HDMI_SOURCE_INPUT2 "ir hdmi source input 2"
#define SZ_IR_HDMI_SOURCE_INPUT3 "ir hdmi source input 3"
#define SZ_IR_HDMI_SOURCE_TOGGLE "ir hdmi source input toggle"
#define SZ_IR_IGNORE             "ir ignore"
#define SZ_IR_MUTE               "ir mute"
#define SZ_IR_VOLUME_DOWN        "ir volume down"
#define SZ_IR_VOLUME_UP          "ir volume up"


// note multiple ir codes exist may exist for the same command 
// because of ir noise emitted from a plasma tv display or when
// keys from two different remotes are mapped to the same command 
//
static const char * undefCharArray[]   = {SZ_UNDEFINED};
static const char * dvdChapterBack[]   = {"7FE81E669E61"};
static const char * dvdChapterNext[]   = {"7F681FE69F61"};
static const char * dvdInfo[]          = {"7F161F989F61"};
static const char * dvdEject[]         = {"7F081F869F61"};
static const char * dvdFastForward[]   = {"7F901E1E9E61"};
static const char * dvdMenu[]          = {"8797E6199E61"}; 
static const char * dvdMenuDown[]      = {"8777E7F99F61"};  
static const char * dvdMenuLeft[]      = {"07F666789E61"};
static const char * dvdMenuRight[]     = {"071667989F61"};
static const char * dvdMenuUp[]        = {"8789E6079E61"};
static const char * dvdN0[]            = {"8769E7E79F61"};
static const char * dvdN1[]            = {"7F961E189E61"};
static const char * dvdN2[]            = {"7F6E1FE09F61"};
static const char * dvdN3[]            = {"7FEE1E209E61"};
static const char * dvdN4[]            = {"7F0E1F809F61"};
static const char * dvdN5[]            = {"7F8E1E009E61"};
static const char * dvdN6[]            = {"8771E7FF9F61"};
static const char * dvdN7[]            = {"87F1E67F9E61"};
static const char * dvdN8[]            = {"8711E79F9F61"};
static const char * dvdN9[]            = {"8791E61F9E61"};
static const char * dvdOk[]            = {"077667F89F61"};
static const char * dvdPause[]         = {"7F761FF89F61"};
static const char * dvdPlay[]          = {"7FF01E7E9E61"};
static const char * dvdPower[]         = {"7F701FFE9F61"};
static const char * dvdRewind[]        = {"7F101F9E9F61"};
static const char * dvdStop[]          = {"7FF61E789E61"};

static const char * hdmiSourceInput1[] = {"218376D55575"};
static const char * hdmiSourceInput2[] = {"218C76DD5575"};
static const char * hdmiSourceInput3[] = {"210C765D5575"};
static const char * hdmiSourceToggle[] = {"212C767D5575"};

static const char * tvChannelDown[]    = {"A28880880003"};
static const char * tvChannelUp[]      = {"A20880080003"};
static const char * tvFastForward[]    = {"2A0808080003"};
static const char * tvN0[]             = {"8282A0820003"};
static const char * tvN1[]             = {"8200A0000003"};
static const char * tvN2[]             = {"8280A0800003"};
static const char * tvN3[]             = {"8220A0200003"};
static const char * tvN4[]             = {"82A0A0A00003"};
static const char * tvN5[]             = {"8208A0080003"};
static const char * tvN6[]             = {"8288A0880003"};
static const char * tvN7[]             = {"8228A0280003"};
static const char * tvN8[]             = {"82A8A0A80003"};
static const char * tvN9[]             = {"8202A0020003"};

static const char * tvDisplay[]       = {"A28280820003"};
static const char * tvMenu[]          = {"8A20A8200003"};
static const char * tvMenuDown[]      = {"0AA228A20003"};
static const char * tvMenuLeft[]      = {"0A2A282A0003"};
static const char * tvMenuRight[]     = {"0AAA28AA0003"};
static const char * tvMenuUp[]        = {"0A2228220003"};
static const char * tvPause[]         = {"8A0AA80A0003"};
static const char * tvPip[]           = {"8A8AA88A0003"};
static const char * tvPlay[]          = {"8A2AA82A0003"};
static const char * tvPower[]         = {"A28A808A0003"};
static const char * tvReturn[]        = {"8808AA080003"};
static const char * tvRewind[]        = {"2A8808880003"};
static const char * tvStop[]          = {"2AA808A80003"};
static const char * tvVideo[]         = {"028820880003"};

static const char * vcrChannelDown[]  = {"025900000000"};
static const char * vcrChannelUp[]    = {"005900000000"};
static const char * vcrFastForward[]  = {"1C1600000000"};
static const char * vcrMenu[]         = {"0C0000000000","0C5000000000"};
static const char * vcrMenuDown[]     = {"3D3001000000","3D0000000000"};
static const char * vcrMenuLeft[]     = {"015000000000","010000000000"};
static const char * vcrMenuRight[]    = {"060000000000","065000000000"};
static const char * vcrMenuUp[]       = {"3F3001000000","3F0000000000"};
static const char * vcrN0[]           = {"005C00000000"};
static const char * vcrN1[]           = {"025C00000000"};
static const char * vcrN2[]           = {"031700000000"};
static const char * vcrN3[]           = {"085C00000000"};
static const char * vcrN4[]           = {"0C1700000000"};
static const char * vcrN5[]           = {"0E1700000000"};
static const char * vcrN6[]           = {"091700000000"};
static const char * vcrN7[]           = {"205C00000000"};
static const char * vcrN8[]           = {"301700000000"};
static const char * vcrN9[]           = {"321700000000"};
static const char * vcrOk[]           = {"1F3001000000","1F0000000000"};
static const char * vcrPause[]        = {"721600000000"};
static const char * vcrPlay[]         = {"1E1600000000"};
static const char * vcrPower[]        = {"231700000000",
                                         "ABBCFDFD555D"};   // xfinity XR11
static const char * vcrRewind[]       = {"131600000000"};
static const char * vcrStop[]         = {"191600000000"};

// round robin interates over TV, DVD and DVR inputs
static const char * vcrVideo[]        = {"015800000000"
                                         "8B13DD55555D"};   // xfinity XR11  

#if 0
// sony vcr command codes
static const char * vcrChannelDown[] = {"3F3F35000000"};
static const char * vcrChannelUp[]   = {"3F3F55000000"};
static const char * vcrFastForward[] = {"3F3F66000000"};
static const char * vcrMenu[]        = {"3FF376000000"};
static const char * vcrMenuDown[]    = {"3FF333000000"};
static const char * vcrMenuLeft[]    = {"3FC353000000"};
static const char * vcrMenuRight[]   = {"3FC335000000"};
static const char * vcrMenuUp[]      = {"3FF353000000"};
static const char * vcrN0[]          = {"3FFF36000000"};
static const char * vcrN1[]          = {"3FFF55000000"};
static const char * vcrN2[]          = {"3FFF35000000"};
static const char * vcrN3[]          = {"3FFF53000000"};
static const char * vcrN4[]          = {"3FFF33000000"};
static const char * vcrN5[]          = {"3FFF65000000"};
static const char * vcrN6[]          = {"3FFF75000000"};
static const char * vcrN7[]          = {"3FFF63000000"};
static const char * vcrN8[]          = {"3FFF73000000"};
static const char * vcrN9[]          = {"3FFF73000000"};
static const char * vcrOk[]          = {"3F3335000000"};
static const char * vcrPause[]       = {"3F3F36000000"};
static const char * vcrPlay[]        = {"3F3F57000000"};
static const char * vcrPower[]       = {"3F3F75000000",
                                        "ABBCFDFD555D"};  // xfinity XR11
static const char * vcrRewind[]      = {"3F3F37000000"};
static const char * vcrStop[]        = {"3F3F56000000"};
#endif

static const char * mute[]       = {"A22080200003",
                                    "AB3CFD7D555D"};   // xfinity XR11
static const char * volumeDown[] = {"228000800003",
                                    "AB1EFD5F555D"};   // xfinity XR11
static const char * volumeUp[]   = {"220000000003",
                                    "AB9EFDDF555D"};   // xfinity XR11
enum tiraId {
   TIRA_UNDEFINED,
   TIRA_DVD_CHAPTER_BACK,
   TIRA_DVD_CHAPTER_NEXT,
   TIRA_DVD_INFO,
   TIRA_DVD_FAST_FORWARD,
   TIRA_DVD_EJECT,
   TIRA_DVD_MENU,
   TIRA_DVD_MENU_DOWN,
   TIRA_DVD_MENU_LEFT,
   TIRA_DVD_MENU_RIGHT,
   TIRA_DVD_MENU_UP,
   TIRA_DVD_N0,
   TIRA_DVD_N1,
   TIRA_DVD_N2,
   TIRA_DVD_N3,
   TIRA_DVD_N4,
   TIRA_DVD_N5,
   TIRA_DVD_N6,
   TIRA_DVD_N7,
   TIRA_DVD_N8,
   TIRA_DVD_N9,
   TIRA_DVD_OK,
   TIRA_DVD_PAUSE,
   TIRA_DVD_PLAY,
   TIRA_DVD_POWER,
   TIRA_DVD_REWIND,
   TIRA_DVD_STOP,
   TIRA_HDMI_SOURCE_INPUT1,
   TIRA_HDMI_SOURCE_INPUT2,
   TIRA_HDMI_SOURCE_INPUT3,
   TIRA_HDMI_SOURCE_TOGGLE,
   TIRA_TV_CHANNEL_DOWN,
   TIRA_TV_CHANNEL_UP,
   TIRA_TV_DISPLAY,
   TIRA_TV_FAST_FORWARD,
   TIRA_TV_RETURN,
   TIRA_TV_REWIND,
   TIRA_TV_MENU,
   TIRA_TV_MENU_DOWN,
   TIRA_TV_MENU_LEFT,
   TIRA_TV_MENU_RIGHT,
   TIRA_TV_MENU_UP,
   TIRA_TV_N0,
   TIRA_TV_N1,
   TIRA_TV_N2,
   TIRA_TV_N3,
   TIRA_TV_N4,
   TIRA_TV_N5,
   TIRA_TV_N6,
   TIRA_TV_N7,
   TIRA_TV_N8,
   TIRA_TV_N9,
   TIRA_TV_PIP,
   TIRA_TV_PAUSE,
   TIRA_TV_PLAY,
   TIRA_TV_POWER,
   TIRA_TV_STOP,
   TIRA_TV_VIDEO,
   TIRA_VCR_CHANNEL_DOWN,
   TIRA_VCR_CHANNEL_UP,
   TIRA_VCR_FAST_FORWARD,
   TIRA_VCR_REWIND,
   TIRA_VCR_MENU,
   TIRA_VCR_MENU_DOWN,
   TIRA_VCR_MENU_LEFT,
   TIRA_VCR_MENU_RIGHT,
   TIRA_VCR_MENU_UP,
   TIRA_VCR_N0,
   TIRA_VCR_N1,
   TIRA_VCR_N2,
   TIRA_VCR_N3,
   TIRA_VCR_N4,
   TIRA_VCR_N5,
   TIRA_VCR_N6,
   TIRA_VCR_N7,
   TIRA_VCR_N8,
   TIRA_VCR_N9,
   TIRA_VCR_OK,
   TIRA_VCR_PAUSE,
   TIRA_VCR_PLAY,
   TIRA_VCR_POWER,
   TIRA_VCR_STOP,
   TIRA_VCR_VIDEO,
   TIRA_VOLUME_DOWN,
   TIRA_VOLUME_UP,
   TIRA_MUTE,
   TIRA_IDS
};


struct irRx {
   int size;            // number of ir code strings
   const char ** code;  // possible ir codes for a particular command
   int matchLen;        // number of chars to qualify as a match
   const char * sz;     // command message and debug description
   DWORD timestamp;     // GetTickCount() when ir cmd last received
};

struct irRxCmd {
   int id;
   struct irRx key;
} irRxCmd[TIRA_IDS] = {
   TIRA_UNDEFINED,
   {sizeof(undefCharArray)/sizeof(char *),undefCharArray,CMD_CODE_LENGTH,SZ_UNDEFINED,0},

   TIRA_DVD_CHAPTER_BACK,
   {sizeof(dvdChapterBack)/sizeof(char *),dvdChapterBack,CMD_CODE_LENGTH,SZ_IR_DVD_CHAPTER_BACK,0},

   TIRA_DVD_CHAPTER_NEXT,
   {sizeof(dvdChapterNext)/sizeof(char *),dvdChapterNext,CMD_CODE_LENGTH,SZ_IR_DVD_CHAPTER_NEXT,0},

   TIRA_DVD_INFO,
   {sizeof(dvdInfo)/sizeof(char *),dvdInfo,CMD_CODE_LENGTH,SZ_IR_DVD_INFO,0},

   TIRA_DVD_EJECT,
   {sizeof(dvdEject)/sizeof(char *),dvdEject,CMD_CODE_LENGTH,SZ_IR_DVD_EJECT,0},

   TIRA_DVD_FAST_FORWARD,
   {sizeof(dvdFastForward)/sizeof(char *),dvdFastForward,CMD_CODE_LENGTH,SZ_IR_DVD_FAST_FORWARD,0},

   TIRA_DVD_MENU,
   {sizeof(dvdMenu)/sizeof(char *),dvdMenu,CMD_CODE_LENGTH,SZ_IR_DVD_MENU,0},

   TIRA_DVD_MENU_DOWN,
   {sizeof(dvdMenuDown)/sizeof(char *),dvdMenuDown,CMD_CODE_LENGTH,SZ_IR_DVD_MENU_DOWN,0},

   TIRA_DVD_MENU_LEFT,
   {sizeof(dvdMenuLeft)/sizeof(char *),dvdMenuLeft,CMD_CODE_LENGTH,SZ_IR_DVD_MENU_LEFT,0},

   TIRA_DVD_MENU_RIGHT,
   {sizeof(dvdMenuRight)/sizeof(char *),dvdMenuRight,CMD_CODE_LENGTH,SZ_IR_DVD_MENU_RIGHT,0},

   TIRA_DVD_MENU_UP,
   {sizeof(dvdMenuUp)/sizeof(char *),dvdMenuUp,CMD_CODE_LENGTH,SZ_IR_DVD_MENU_UP,0},

   TIRA_DVD_N0,
   {sizeof(dvdN0)/sizeof(char *),dvdN0,CMD_CODE_LENGTH,SZ_IR_DVD_N0,0},

   TIRA_DVD_N1,
   {sizeof(dvdN1)/sizeof(char *),dvdN1,CMD_CODE_LENGTH,SZ_IR_DVD_N1,0},

   TIRA_DVD_N2,
   {sizeof(dvdN2)/sizeof(char *),dvdN2,CMD_CODE_LENGTH,SZ_IR_DVD_N2,0},

   TIRA_DVD_N3,
   {sizeof(dvdN3)/sizeof(char *),dvdN3,CMD_CODE_LENGTH,SZ_IR_DVD_N3,0},

   TIRA_DVD_N4,
   {sizeof(dvdN4)/sizeof(char *),dvdN4,CMD_CODE_LENGTH,SZ_IR_DVD_N4,0},

   TIRA_DVD_N5,
   {sizeof(dvdN5)/sizeof(char *),dvdN5,CMD_CODE_LENGTH,SZ_IR_DVD_N5,0},

   TIRA_DVD_N6,
   {sizeof(dvdN6)/sizeof(char *),dvdN6,CMD_CODE_LENGTH,SZ_IR_DVD_N6,0},

   TIRA_DVD_N7,
   {sizeof(dvdN7)/sizeof(char *),dvdN7,CMD_CODE_LENGTH,SZ_IR_DVD_N7,0},

   TIRA_DVD_N8,
   {sizeof(dvdN8)/sizeof(char *),dvdN8,CMD_CODE_LENGTH,SZ_IR_DVD_N8,0},

   TIRA_DVD_N9,
   {sizeof(dvdN9)/sizeof(char *),dvdN9,CMD_CODE_LENGTH,SZ_IR_DVD_N9,0},

   TIRA_DVD_OK,
   {sizeof(dvdOk)/sizeof(char *),dvdOk,CMD_CODE_LENGTH,SZ_IR_DVD_OK,0},

   TIRA_DVD_PAUSE,
   {sizeof(dvdPause)/sizeof(char *),dvdPause,CMD_CODE_LENGTH,SZ_IR_DVD_PAUSE,0},

   TIRA_DVD_PLAY,
   {sizeof(dvdPlay)/sizeof(char *),dvdPlay,CMD_CODE_LENGTH,SZ_IR_DVD_PLAY,0},

   TIRA_DVD_POWER,
   {sizeof(dvdPower)/sizeof(char *),dvdPower,CMD_CODE_LENGTH,SZ_IR_DVD_POWER,0},

   TIRA_DVD_REWIND,
   {sizeof(dvdRewind)/sizeof(char *),dvdRewind,CMD_CODE_LENGTH,SZ_IR_DVD_REWIND,0},

   TIRA_DVD_STOP,
   {sizeof(dvdStop)/sizeof(char *),dvdStop,CMD_CODE_LENGTH,SZ_IR_DVD_STOP,0},

   TIRA_HDMI_SOURCE_INPUT1,
   {sizeof(hdmiSourceInput1)/sizeof(char *),hdmiSourceInput1,CMD_CODE_LENGTH,SZ_IR_HDMI_SOURCE_INPUT1,0},

   TIRA_HDMI_SOURCE_INPUT2,
   {sizeof(hdmiSourceInput2)/sizeof(char *),hdmiSourceInput2,CMD_CODE_LENGTH,SZ_IR_HDMI_SOURCE_INPUT2,0},

   TIRA_HDMI_SOURCE_INPUT3,
   {sizeof(hdmiSourceInput3)/sizeof(char *),hdmiSourceInput3,CMD_CODE_LENGTH,SZ_IR_HDMI_SOURCE_INPUT3,0},

   TIRA_HDMI_SOURCE_TOGGLE,
   {sizeof(hdmiSourceToggle)/sizeof(char *),hdmiSourceToggle,CMD_CODE_LENGTH,SZ_IR_HDMI_SOURCE_TOGGLE,0},

   TIRA_TV_CHANNEL_DOWN,
   {sizeof(tvChannelDown)/sizeof(char *),tvChannelDown,CMD_CODE_LENGTH,SZ_IR_TV_CHANNEL_DOWN,0},

   TIRA_TV_CHANNEL_UP,
   {sizeof(tvChannelUp)/sizeof(char *),tvChannelUp,CMD_CODE_LENGTH,SZ_IR_TV_CHANNEL_UP,0},

   TIRA_TV_DISPLAY,
   {sizeof(tvDisplay)/sizeof(char *),tvDisplay,CMD_CODE_LENGTH,SZ_IR_TV_DISPLAY,0},

   TIRA_TV_FAST_FORWARD,
   {sizeof(tvFastForward)/sizeof(char *),tvFastForward,CMD_CODE_LENGTH,SZ_IR_TV_FAST_FORWARD,0},

   TIRA_TV_RETURN,
   {sizeof(tvReturn)/sizeof(char *),tvReturn,CMD_CODE_LENGTH,SZ_IR_TV_RETURN,0},

   TIRA_TV_REWIND,
   {sizeof(tvRewind)/sizeof(char *),tvRewind,CMD_CODE_LENGTH,SZ_IR_TV_REWIND,0},

   TIRA_TV_MENU,
   {sizeof(tvMenu)/sizeof(char *),tvMenu,CMD_CODE_LENGTH,SZ_IR_TV_MENU,0},

   TIRA_TV_MENU_DOWN,
   {sizeof(tvMenuDown)/sizeof(char *),tvMenuDown,CMD_CODE_LENGTH,SZ_IR_TV_MENU_DOWN,0},

   TIRA_TV_MENU_LEFT,
   {sizeof(tvMenuLeft)/sizeof(char *),tvMenuLeft,CMD_CODE_LENGTH,SZ_IR_TV_MENU_LEFT,0},

   TIRA_TV_MENU_RIGHT,
   {sizeof(tvMenuRight)/sizeof(char *),tvMenuRight,CMD_CODE_LENGTH,SZ_IR_TV_MENU_RIGHT,0},

   TIRA_TV_MENU_UP,
   {sizeof(tvMenuUp)/sizeof(char *),tvMenuUp,CMD_CODE_LENGTH,SZ_IR_TV_MENU_UP,0},

   TIRA_TV_N0,
   {sizeof(tvN0)/sizeof(char *),tvN0,CMD_CODE_LENGTH,SZ_IR_TV_N0,0},

   TIRA_TV_N1,
   {sizeof(tvN1)/sizeof(char *),tvN1,CMD_CODE_LENGTH,SZ_IR_TV_N1,0},

   TIRA_TV_N2,
   {sizeof(tvN2)/sizeof(char *),tvN2,CMD_CODE_LENGTH,SZ_IR_TV_N2,0},

   TIRA_TV_N3,
   {sizeof(tvN3)/sizeof(char *),tvN3,CMD_CODE_LENGTH,SZ_IR_TV_N3,0},

   TIRA_TV_N4,
   {sizeof(tvN4)/sizeof(char *),tvN4,CMD_CODE_LENGTH,SZ_IR_TV_N4,0},

   TIRA_TV_N5,
   {sizeof(tvN5)/sizeof(char *),tvN5,CMD_CODE_LENGTH,SZ_IR_TV_N5,0},

   TIRA_TV_N6,
   {sizeof(tvN6)/sizeof(char *),tvN6,CMD_CODE_LENGTH,SZ_IR_TV_N6,0},

   TIRA_TV_N7,
   {sizeof(tvN7)/sizeof(char *),tvN7,CMD_CODE_LENGTH,SZ_IR_TV_N7,0},

   TIRA_TV_N8,
   {sizeof(tvN8)/sizeof(char *),tvN8,CMD_CODE_LENGTH,SZ_IR_TV_N8,0},

   TIRA_TV_N9,
   {sizeof(tvN9)/sizeof(char *),tvN9,CMD_CODE_LENGTH,SZ_IR_TV_N9,0},

   TIRA_TV_PIP,
   {sizeof(tvPip)/sizeof(char *),tvPip,CMD_CODE_LENGTH,SZ_IR_TV_PIP,0},

   TIRA_TV_PAUSE,
   {sizeof(tvPause)/sizeof(char *),tvPause,CMD_CODE_LENGTH,SZ_IR_TV_PAUSE,0},

   TIRA_TV_PLAY,
   {sizeof(tvPlay)/sizeof(char *),tvPlay,CMD_CODE_LENGTH,SZ_IR_TV_PLAY,0},

   TIRA_TV_POWER,
   {sizeof(tvPower)/sizeof(char *),tvPower,CMD_CODE_LENGTH,SZ_IR_TV_POWER,0},

   TIRA_TV_STOP,
   {sizeof(tvStop)/sizeof(char *),tvStop,CMD_CODE_LENGTH,SZ_IR_TV_STOP,0},

   TIRA_TV_VIDEO,
   {sizeof(tvVideo)/sizeof(char *),tvVideo,CMD_CODE_LENGTH,SZ_IR_TV_VIDEO,0},

   TIRA_VCR_CHANNEL_DOWN,
   {sizeof(vcrChannelDown)/sizeof(char *),vcrChannelDown,CMD_CODE_LENGTH,SZ_IR_VCR_CHANNEL_DOWN,0},

   TIRA_VCR_CHANNEL_UP,
   {sizeof(vcrChannelUp)/sizeof(char *),vcrChannelUp,CMD_CODE_LENGTH,SZ_IR_VCR_CHANNEL_UP,0},

   TIRA_VCR_FAST_FORWARD,
   {sizeof(vcrFastForward)/sizeof(char *),vcrFastForward,CMD_CODE_LENGTH,SZ_IR_VCR_FAST_FORWARD,0},

   TIRA_VCR_REWIND,
   {sizeof(vcrRewind)/sizeof(char *),vcrRewind,CMD_CODE_LENGTH,SZ_IR_VCR_REWIND,0},

   TIRA_VCR_MENU,
   {sizeof(vcrMenu)/sizeof(char *),vcrMenu,CMD_CODE_LENGTH,SZ_IR_VCR_MENU,0},

   TIRA_VCR_MENU_DOWN,
   {sizeof(vcrMenuDown)/sizeof(char *),vcrMenuDown,CMD_CODE_LENGTH,SZ_IR_VCR_MENU_DOWN,0},

   TIRA_VCR_MENU_LEFT,
   {sizeof(vcrMenuLeft)/sizeof(char *),vcrMenuLeft,CMD_CODE_LENGTH,SZ_IR_VCR_MENU_LEFT,0},

   TIRA_VCR_MENU_RIGHT,
   {sizeof(vcrMenuRight)/sizeof(char *),vcrMenuRight,CMD_CODE_LENGTH,SZ_IR_VCR_MENU_RIGHT,0},

   TIRA_VCR_MENU_UP,
   {sizeof(vcrMenuUp)/sizeof(char *),vcrMenuUp,CMD_CODE_LENGTH,SZ_IR_VCR_MENU_UP,0},

   TIRA_VCR_N0,
   {sizeof(vcrN0)/sizeof(char *),vcrN0,CMD_CODE_LENGTH,SZ_IR_VCR_N0,0},

   TIRA_VCR_N1,
   {sizeof(vcrN1)/sizeof(char *),vcrN1,CMD_CODE_LENGTH,SZ_IR_VCR_N1,0},

   TIRA_VCR_N2,
   {sizeof(vcrN2)/sizeof(char *),vcrN2,CMD_CODE_LENGTH,SZ_IR_VCR_N2,0},

   TIRA_VCR_N3,
   {sizeof(vcrN3)/sizeof(char *),vcrN3,CMD_CODE_LENGTH,SZ_IR_VCR_N3,0},

   TIRA_VCR_N4,
   {sizeof(vcrN4)/sizeof(char *),vcrN4,CMD_CODE_LENGTH,SZ_IR_VCR_N4,0},

   TIRA_VCR_N5,
   {sizeof(vcrN5)/sizeof(char *),vcrN5,CMD_CODE_LENGTH,SZ_IR_VCR_N5,0},

   TIRA_VCR_N6,
   {sizeof(vcrN6)/sizeof(char *),vcrN6,CMD_CODE_LENGTH,SZ_IR_VCR_N6,0},

   TIRA_VCR_N7,
   {sizeof(vcrN7)/sizeof(char *),vcrN7,CMD_CODE_LENGTH,SZ_IR_VCR_N7,0},

   TIRA_VCR_N8,
   {sizeof(vcrN8)/sizeof(char *),vcrN8,CMD_CODE_LENGTH,SZ_IR_VCR_N8,0},

   TIRA_VCR_N9,
   {sizeof(vcrN9)/sizeof(char *),vcrN9,CMD_CODE_LENGTH,SZ_IR_VCR_N9,0},

   TIRA_VCR_OK,
   {sizeof(vcrOk)/sizeof(char *),vcrOk,CMD_CODE_LENGTH,SZ_IR_VCR_OK,0},

   TIRA_VCR_PAUSE,
   {sizeof(vcrPause)/sizeof(char *),vcrPause,CMD_CODE_LENGTH,SZ_IR_VCR_PAUSE,0},

   TIRA_VCR_PLAY,
   {sizeof(vcrPlay)/sizeof(char *),vcrPlay,CMD_CODE_LENGTH,SZ_IR_VCR_PLAY,0},

   TIRA_VCR_POWER,
   {sizeof(vcrPower)/sizeof(char *),vcrPower,CMD_CODE_LENGTH,SZ_IR_VCR_POWER,0},

   TIRA_VCR_STOP,
   {sizeof(vcrStop)/sizeof(char *),vcrStop,CMD_CODE_LENGTH,SZ_IR_VCR_STOP,0},

   TIRA_VCR_VIDEO,
   {sizeof(vcrVideo)/sizeof(char *),vcrVideo,CMD_CODE_LENGTH,SZ_IR_VCR_VIDEO,0},

   TIRA_VOLUME_DOWN,
   {sizeof(volumeDown)/sizeof(char *),volumeDown,CMD_CODE_LENGTH,SZ_IR_VOLUME_DOWN,0},

   TIRA_VOLUME_UP,
   {sizeof(volumeUp)/sizeof(char *),volumeUp,CMD_CODE_LENGTH,SZ_IR_VOLUME_UP,0},

   TIRA_MUTE,
   {sizeof(mute)/sizeof(char *),mute,CMD_CODE_LENGTH,SZ_IR_MUTE,0}
};


