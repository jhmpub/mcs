////
//// Tira encoded IR messages received from a universal remote
////

// cmd codes are 12 chars but matching the first 4 is unique enough to aid matching
#define CMD_CODE_LENGTH          4

#define SZ_IR_DVD_DISPLAY        "ir dvd display"
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

static const char * undefCharArray[] = {
SZ_UNDEFINED
};

static const char * dvdDisplay[] = {
"0B2C7E7D77F5"
};

static const char * dvdEnt[] = {
"0B967ED577FF"
};

static const char * dvdFastForward[] = {
"0B8C7EDD77F5"
};

static const char * dvdMenu[] = {
"0B2C7E7D77F5"
};

static const char * dvdMenuDown[] = {
"0B2C7E7D77F5"
};

static const char * dvdMenuLeft[] = {
"0B2C7E7D77F5"
};

static const char * dvdMenuRight[] = {
"0B2C7E7D77F5"
};

static const char * dvdMenuUp[] = {
"0B2C7E7D77F5"
};

static const char * dvdN0[] = {
"0B2C7E7D77F5"
};

static const char * dvdN1[] = {
"0B2C7E7D77F5"
};

static const char * dvdN2[] = {
"0B2C7E7D77F5"
};

static const char * dvdN3[] = {
"0B2C7E7D77F5"
};

static const char * dvdN4[] = {
"0B2C7E7D77F5"
};

static const char * dvdN5[] = {
"0B2C7E7D77F5"
};

static const char * dvdN6[] = {
"0B2C7E7D77F5"
};

static const char * dvdN7[] = {
"0B2C7E7D77F5"
};

static const char * dvdN8[] = {
"0B2C7E7D77F5"
};

static const char * dvdN9[] = {
"0B2C7E7D77F5"
};

static const char * dvdOk[] = {
"0B2C7E7D77F5"
};

static const char * dvdPause[] = {
"0B037E5577F5"
};

static const char * dvdPlay[] = {
"0B837ED577F5"
};

static const char * dvdPower[] = {
"0B2C7E7D77F5"
};

static const char * dvdRewind[] = {
"0B2C7E7D77F5",
"0B0C7E5D77F5"
};

static const char * dvdStop[] = {
"0BAC7EFD77F5"
};

static const char * hdmiSourceInput1[] = {
"218376D55575"
};

static const char * hdmiSourceInput2[] = {
"218C76DD5575"
};

static const char * hdmiSourceInput3[] = {
"210C765D5575"
};

static const char * hdmiSourceToggle[] = {
"212C767D5575"
};

static const char * tvChannelDown[] = {
"A08880880003",
"A08980880003",
"A09880880003",
"A28880880003",
"A28980880103",
"A29880980003",
"A38880890003",
"A289C0880003",
"A68880880003",
"A68980880003",
"E288C0880103",
"E28980880003",
"E298C0880003",
"E299C0880003",
"E3B201000000",
"E68880890003"
};

static const char * tvChannelUp[] = {
"A008C0080003",
"A018C0090003",
"A20880080003",
"A20980080003",
"A21880080003",
"A21980080003",
"A30880080003",
"A30980090003",
"A30980090003",
"A318C0080003",
"A60880080003",
"A609C0080003",
"A61880080003",
"A61980080003",
"E00880080003",
"E009C0180003",
"E20880180003",
"E20980080003",
"E21880090003",
"E21980080003",
"E30980080003",
"E609C0080003"
};

static const char * tvDisplay[] = {
"A28280820003",
"A28380820003",
"A08280820003",
"A083C0830003",
"A08680820003",
"A68780820003",
"A68280820103",
"A68380820003",
"A68680820003",
"E28280820003",
"E283C0820003",
"E28680820103",
"A28380820003",
"A28680820003",
"A28780820003",
};

static const char * tvFastForward[] = {
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

static const char * tvMenu[] = {
"8A20A8200003",
"8A60E8210003",
"8AE0A8200003",
"8E20A8610003",
"9A21B8210003",
"9C21B8200003",
"9E20A8200003",
"9E21B8210003"
};

static const char * tvMenuDown[] = {
"0AA228A20003"
};

static const char * tvMenuLeft[] = {
"0A2A282A0003"
};

static const char * tvMenuRight[] = {
"0AAA28AA0003"
};

static const char * tvMenuUp[] = {
"0A2228220003"
};

static const char * tvN0[] = {
"8282A0820003"
};

static const char * tvN1[] = {
"8000A0000003",
"8001A0000003",
"8200A0000003",
"8201A0000003",
"8300A0000003",
"8600A0000003",
"8601E0010003"
};

static const char * tvN2[] = {
"8080A0800003",
"8280A0800003",
"8281A0800003"
};

static const char * tvN3[] = {
"8220A0200003"
};

static const char * tvN4[] = {
"82A0A0A00003"
};

static const char * tvN5[] = {
"8008A0080003",
"8009A0180003",
"8208A0080003",
"8218A0080003",
"8608E0080003",
};

static const char * tvN6[] = {
"8288A0880003",
"8298E1880003"
};

static const char * tvN7[] = {
"8228A0280003",
"8229A0380003"
};

static const char * tvN8[] = {
"80A8A0A80003",
"80B8A0A90003",
"82A8A0A80003",
"82B8A0A80103",
"82E8A0A80103",
"86A8A0A80003",
"86B8A1E90003",
};

static const char * tvN9[] = {
"8202A0020003"
};


static const char * tvPause[] = {
"8A0AA80A0003",
"8A1AA80A0003",
"880AA80A0003",
"9A0AA80A0003",
"8A0BA80A0003"
};

static const char * tvPip[] = {
"8A8AA88A0003",
"8A8BA88A0003",
"8E8AA88A0003"
};

static const char * tvPlay[] = {
"8A2AA82A0003",
"8A2AB82A0003"
};

static const char * tvPower[] = {
"A08A808A0003",
"A08B808B0003",
"A08E808A0003",
"A09A808E0003",
"A09B809E0003",
"A28A808A0003",
"A28B808A0003",
"A28E808A0003",
"A28F809A0003",
"A29A808A0003",
"A29B808A0103",
"A29E808E0003",
"A38AA3220081",
"A38B808A0003",
"A38E818A0003",
"A68AC08A0003",
"A68BC08A0103",
"A69A808A0003",
"A68E808A0003",
"E08AC08A0003",
"E08E808A0003",
"E09BC19F0103",
"E09F809F0103",
"E28A818E0003",
"E28BC08A0103",
"E28F808E0103",
"E29A808A0003",
"E29BC19F0103",
"E68A808A0003",
"E68E808A0003",
"E68F809A0103",
"E69AC08A0003"
};

static const char * tvReturn[] = {
"8808AA080003",
"8809AA080003",
"8818AA080003",
"8819FA180003",
"8A08AA080003",
"8A09EA180003",
"8C08AA080003",
"8C19AA180003",
"900BA60B0060",
"9808AA080003",
"9809AA080003",
"9818BE090003",
"9819BE080003",
"9A09BE080003",
"9A18FE090003",
"9A19BE090003",
"9C09FE090003",
"9C18EA080003",
"9C19FE190003",
"9E09BA090003",
"9E19BE190003"
};

static const char * tvRewind[] = {
"2A8808880003",
"2A8809880003",
"2A8818880003",
"2E8808880003",
"6A8808880003",
"288808880003",
};

static const char * tvStop[] = {
"2AA808A80003",
"2EA808A80003",
"6AA808A80003",
"2AE808A80003",
"2AB808A80003",
"2AA838A80003"
};

static const char * tvVideo[] = {
"008820880003",
"028820880003",
"029820880003",
"068820880003"
};

static const char * vcrChannelDown[] = {
"025900000000"
};

static const char * vcrChannelUp[] = {
"005900000000" 
};

static const char * vcrFastForward[] = {
"1C1600000000"
};

// calls setNextForegroundWindow()
static const char * vcrMenu[] = {
"0C0000000000",
"0C5000000000"
};

static const char * vcrMenuDown[] = {
"3D3001000000",
"3D0000000000"
};

static const char * vcrMenuLeft[] = {
"015000000000",
"010000000000"
};

static const char * vcrMenuRight[] = {
"060000000000",
"065000000000"
};

static const char * vcrMenuUp[] = {
"3F3001000000",
"3F0000000000"
};

static const char * vcrN0[] = {
"005C00000000"
};

static const char * vcrN1[] = {
"025C00000000"
};

static const char * vcrN2[] = {
"031700000000"
};

static const char * vcrN3[] = {
"085C00000000"
};

static const char * vcrN4[] = {
"0C1700000000"
};

static const char * vcrN5[] = {
"0E1700000000"
};

static const char * vcrN6[] = {
"091700000000"
};

static const char * vcrN7[] = {
"205C00000000"
};

static const char * vcrN8[] = {
"301700000000"
};

static const char * vcrN9[] = {
"321700000000"
};

static const char * vcrOk[] = {
"1F3001000000",
"1F0000000000"
};

static const char * vcrPause[] = {
"721600000000"
};

static const char * vcrPlay[] = {
"1E1600000000"
};

static const char * vcrPower[] = {
"ABBCFDFD555D",   // xfinity XR11
"231700000000"
};

static const char * vcrRewind[] = {
"131600000000"
};

static const char * vcrStop[] = {
"191600000000"
};

// round robin interates over TV, DVD and DVR inputs
static const char * vcrVideo[] = {
"8B13DD55555D",   // xfinity XR11  
"015800000000"
};

static const char * mute[] = {
"AB3CFD7D555D",   // xfinity XR11
"A02180200003",
"A02080200003",
"A06080600003",
"A22080200003",
"A22180200003",
"A260C0200003",
"A26180200003",
"A320C0600003",
"A32180200003",
"A62080200003",
"A62180200003",
"A660C0200003",
"A661C0200003",
"E02080200003",
"E021C0210003",
"E06180610003",
"E22080600003",
"E32080200003",
"E620C0200003",
"E621C0210003",
};

static const char * volumeDown[] = {
"AB1EFD5F555D",   // xfinity XR11
"228000800003",
"E180038003BC",
"608001800103",
"628000800003",
"668031800003",
"208000800003",
"638000800003",
"268000800003",
"238000800003"
};

static const char * volumeUp[] = {
"AB9EFDDF555D",   // xfinity XR11
"220000000003"
};


struct irRx {
   int size;            // number of coded ir strings
   const char ** code;  // possible ir codes for a particular rcCmd
   const char * sz;     // command message and debug description
   DWORD timestamp;     // GetTickCount() when ir cmd last received
};

static struct irRx irRxUndefined = {
   sizeof(undefCharArray)/sizeof(char *),
   undefCharArray,
   SZ_UNDEFINED
};   

static struct irRx irRxDvdMenu = {
   sizeof(dvdMenu)/sizeof(char *),
   dvdMenu,
   SZ_IR_DVD_MENU
};
   
static struct irRx irRxDvdMenuDown = {
   sizeof(dvdMenuDown)/sizeof(char *),
   dvdMenuDown,
   SZ_IR_DVD_MENU_DOWN
};
   
static struct irRx irRxDvdMenuLeft = {
   sizeof(dvdMenuLeft)/sizeof(char *),
   dvdMenuLeft,
   SZ_IR_DVD_MENU_LEFT
};

static struct irRx irRxDvdMenuRight = {
   sizeof(dvdMenuRight)/sizeof(char *),
   dvdMenuRight,
   SZ_IR_DVD_MENU_RIGHT
};
   
static struct irRx irRxDvdMenuUp = {
   sizeof(dvdMenuUp)/sizeof(char *),
   dvdMenuUp,
   SZ_IR_DVD_MENU_UP
};

static struct irRx irRxDvdOk = {
   sizeof(dvdOk)/sizeof(char *),
   dvdOk,
   SZ_IR_DVD_OK
};

static struct irRx irRxDvdDisplay = {
   sizeof(dvdDisplay)/sizeof(char *),
   dvdDisplay,
   SZ_IR_DVD_DISPLAY
};

static struct irRx irRxDvdEject = {
   sizeof(dvdEnt)/sizeof(char *),
   dvdEnt,
   SZ_IR_DVD_EJECT
};

static struct irRx irRxDvdFastForward = {
   sizeof(dvdFastForward)/sizeof(char *),
   dvdFastForward,
   SZ_IR_DVD_FAST_FORWARD
};

static struct irRx irRxDvdN0 = {
   sizeof(dvdN0)/sizeof(char *),
   dvdN0,
   SZ_IR_DVD_N0
};

static struct irRx irRxDvdN1 = {
   sizeof(dvdN1)/sizeof(char *),
   dvdN1,
   SZ_IR_DVD_N1
};

static struct irRx irRxDvdN2 = {
   sizeof(dvdN2)/sizeof(char *),
   dvdN2,
   SZ_IR_DVD_N2
};

static struct irRx irRxDvdN3 = {
   sizeof(dvdN3)/sizeof(char *),
   dvdN3,
   SZ_IR_DVD_N3
};

static struct irRx irRxDvdN4 = {
   sizeof(dvdN4)/sizeof(char *),
   dvdN4,
   SZ_IR_DVD_N4
};

static struct irRx irRxDvdN5 = {
   sizeof(dvdN5)/sizeof(char *),
   dvdN5,
   SZ_IR_DVD_N5
};

static struct irRx irRxDvdN6 = {
   sizeof(dvdN6)/sizeof(char *),
   dvdN6,
   SZ_IR_DVD_N6
};

static struct irRx irRxDvdN7 = {
   sizeof(dvdN7)/sizeof(char *),
   dvdN7,
   SZ_IR_DVD_N7
};

static struct irRx irRxDvdN8 = {
   sizeof(dvdN8)/sizeof(char *),
   dvdN8,
   SZ_IR_DVD_N8
};

static struct irRx irRxDvdN9 = {
   sizeof(dvdN9)/sizeof(char *),
   dvdN9,
   SZ_IR_DVD_N9
};

static struct irRx irRxDvdPause = {
   sizeof(dvdPause)/sizeof(char *),
   dvdPause,
   SZ_IR_DVD_PAUSE
};

static struct irRx irRxDvdPlay = {
   sizeof(dvdPlay)/sizeof(char *),
   dvdPlay,
   SZ_IR_DVD_PLAY
};

static struct irRx irRxDvdPower = {
   sizeof(dvdPower)/sizeof(char *),
   dvdPower,
   SZ_IR_DVD_POWER
};

static struct irRx irRxDvdRewind = {
   sizeof(dvdRewind)/sizeof(char *),
   dvdRewind,
   SZ_IR_DVD_REWIND
};

static struct irRx irRxDvdStop = {
   sizeof(dvdStop)/sizeof(char *),
   dvdStop,
   SZ_IR_DVD_STOP
};

static struct irRx irRxHdmiSourceInput1 = {
   sizeof(hdmiSourceInput1)/sizeof(char *),
   hdmiSourceInput1,
   SZ_IR_HDMI_SOURCE_INPUT1
};

static struct irRx irRxHdmiSourceInput2 = {
   sizeof(hdmiSourceInput2)/sizeof(char *),
   hdmiSourceInput2,
   SZ_IR_HDMI_SOURCE_INPUT2
};

static struct irRx irRxHdmiSourceInput3 = {
   sizeof(hdmiSourceInput3)/sizeof(char *),
   hdmiSourceInput3,
   SZ_IR_HDMI_SOURCE_INPUT3
};

static struct irRx irRxHdmiSourceToggle = {
   sizeof(hdmiSourceToggle)/sizeof(char *),
   hdmiSourceToggle,
   SZ_IR_HDMI_SOURCE_TOGGLE
};

static struct irRx irRxTvChannelDown = {
   sizeof(tvChannelDown)/sizeof(char *),
   tvChannelDown,
   SZ_IR_TV_CHANNEL_DOWN
};
   
static struct irRx irRxTvChannelUp = {
   sizeof(tvChannelUp)/sizeof(char *),
   tvChannelUp,
   SZ_IR_TV_CHANNEL_UP
};
   
static struct irRx irRxTvDisplay = {
   sizeof(tvDisplay)/sizeof(char *),
   tvDisplay,
   SZ_IR_TV_DISPLAY
};

static struct irRx irRxTvFastForward = {
   sizeof(tvFastForward)/sizeof(char *),
   tvFastForward,
   SZ_IR_TV_FAST_FORWARD
};

static struct irRx irRxTvReturn = {
   sizeof(tvReturn)/sizeof(char *),
   tvReturn,
   SZ_IR_TV_RETURN
};

static struct irRx irRxTvRewind = {
   sizeof(tvRewind)/sizeof(char *),
   tvRewind,
   SZ_IR_TV_REWIND
};

static struct irRx irRxTvMenu = {
   sizeof(tvMenu)/sizeof(char *),
   tvMenu,
   SZ_IR_TV_MENU
};

static struct irRx irRxTvMenuDown = {
   sizeof(tvMenuDown)/sizeof(char *),
   tvMenuDown,
   SZ_IR_TV_MENU_DOWN
};

static struct irRx irRxTvMenuLeft = {
   sizeof(tvMenuLeft)/sizeof(char *),
   tvMenuLeft,
   SZ_IR_TV_MENU_LEFT
};

static struct irRx irRxTvMenuRight = {
   sizeof(tvMenuRight)/sizeof(char *),
   tvMenuRight,
   SZ_IR_TV_MENU_RIGHT
};

static struct irRx irRxTvMenuUp = {
   sizeof(tvMenuUp)/sizeof(char *),
   tvMenuUp,
   SZ_IR_TV_MENU_UP
};

static struct irRx irRxTvN0 = {
   sizeof(tvN0)/sizeof(char *),
   tvN0,
   SZ_IR_TV_N0
};

static struct irRx irRxTvN1 = {
   sizeof(tvN1)/sizeof(char *),
   tvN1,
   SZ_IR_TV_N1
};

static struct irRx irRxTvN2 = {
   sizeof(tvN2)/sizeof(char *),
   tvN2,
   SZ_IR_TV_N2
};

static struct irRx irRxTvN3 = {
   sizeof(tvN3)/sizeof(char *),
   tvN3,
   SZ_IR_TV_N3
};

static struct irRx irRxTvN4 = {
   sizeof(tvN4)/sizeof(char *),
   tvN4,
   SZ_IR_TV_N4
};

static struct irRx irRxTvN5 = {
   sizeof(tvN5)/sizeof(char *),
   tvN5,
   SZ_IR_TV_N5
};

static struct irRx irRxTvN6 = {
   sizeof(tvN6)/sizeof(char *),
   tvN6,
   SZ_IR_TV_N6
};

static struct irRx irRxTvN7 = {
   sizeof(tvN7)/sizeof(char *),
   tvN7,
   SZ_IR_TV_N7
};

static struct irRx irRxTvN8 = {
   sizeof(tvN8)/sizeof(char *),
   tvN8,
   SZ_IR_TV_N8
};

static struct irRx irRxTvN9 = {
   sizeof(tvN9)/sizeof(char *),
   tvN9,
   SZ_IR_TV_N9
};

static struct irRx irRxTvPip = {
   sizeof(tvPip)/sizeof(char *),
   tvPip,
   SZ_IR_TV_PIP
};

static struct irRx irRxTvPause = {
   sizeof(tvPause)/sizeof(char *),
   tvPause,
   SZ_IR_TV_PAUSE
};

static struct irRx irRxTvPlay = {
   sizeof(tvPlay)/sizeof(char *),
   tvPlay,
   SZ_IR_TV_PLAY
};

static struct irRx irRxTvPower = {
   sizeof(tvPower)/sizeof(char *),
   tvPower,
   SZ_IR_TV_POWER
};

static struct irRx irRxTvStop = {
   sizeof(tvStop)/sizeof(char *),
   tvStop,
   SZ_IR_TV_STOP
};

static struct irRx irRxTvVideo = {
   sizeof(tvVideo)/sizeof(char *),
   tvVideo,
   SZ_IR_TV_VIDEO
};

static struct irRx irRxVcrChannelDown = {
   sizeof(vcrChannelDown)/sizeof(char *),
   vcrChannelDown,
   SZ_IR_VCR_CHANNEL_DOWN
};
   
static struct irRx irRxVcrChannelUp = {
   sizeof(vcrChannelUp)/sizeof(char *),
   vcrChannelUp,
   SZ_IR_VCR_CHANNEL_UP
};
   
static struct irRx irRxVcrFastForward = {
   sizeof(vcrFastForward)/sizeof(char *),
   vcrFastForward,
   SZ_IR_VCR_FAST_FORWARD
};

static struct irRx irRxVcrMenu = {
   sizeof(vcrMenu)/sizeof(char *),
   vcrMenu,
   SZ_IR_VCR_MENU
};
   
static struct irRx irRxVcrMenuDown = {
   sizeof(vcrMenuDown)/sizeof(char *),
   vcrMenuDown,
   SZ_IR_VCR_MENU_DOWN
};
   
static struct irRx irRxVcrMenuLeft = {
   sizeof(vcrMenuLeft)/sizeof(char *),
   vcrMenuLeft,
   SZ_IR_VCR_MENU_LEFT
};

static struct irRx irRxVcrMenuRight = {
   sizeof(vcrMenuRight)/sizeof(char *),
   vcrMenuRight,
   SZ_IR_VCR_MENU_RIGHT
};
   
static struct irRx irRxVcrMenuUp = {
   sizeof(vcrMenuUp)/sizeof(char *),
   vcrMenuUp,
   SZ_IR_VCR_MENU_UP
};

static struct irRx irRxMute = {
   sizeof(mute)/sizeof(char *),
   mute,
   SZ_IR_MUTE
};

static struct irRx irRxVcrN0 = {
   sizeof(vcrN0)/sizeof(char *),
   vcrN0,
   SZ_IR_VCR_N0
};

static struct irRx irRxVcrN1 = {
   sizeof(vcrN1)/sizeof(char *),
   vcrN1,
   SZ_IR_VCR_N1
};

static struct irRx irRxVcrN2 = {
   sizeof(vcrN2)/sizeof(char *),
   vcrN2,
   SZ_IR_VCR_N2
};

static struct irRx irRxVcrN3 = {
   sizeof(vcrN3)/sizeof(char *),
   vcrN3,
   SZ_IR_VCR_N3
};

static struct irRx irRxVcrN4 = {
   sizeof(vcrN4)/sizeof(char *),
   vcrN4,
   SZ_IR_VCR_N4
};

static struct irRx irRxVcrN5 = {
   sizeof(vcrN5)/sizeof(char *),
   vcrN5,
   SZ_IR_VCR_N5
};

static struct irRx irRxVcrN6 = {
   sizeof(vcrN6)/sizeof(char *),
   vcrN6,
   SZ_IR_VCR_N6
};

static struct irRx irRxVcrN7 = {
   sizeof(vcrN7)/sizeof(char *),
   vcrN7,
   SZ_IR_VCR_N7
};

static struct irRx irRxVcrN8 = {
   sizeof(vcrN8)/sizeof(char *),
   vcrN8,
   SZ_IR_VCR_N8
};

static struct irRx irRxVcrN9 = {
   sizeof(vcrN9)/sizeof(char *),
   vcrN9,
   SZ_IR_VCR_N9
};

static struct irRx irRxVcrOk = {
   sizeof(vcrOk)/sizeof(char *),
   vcrOk,
   SZ_IR_VCR_OK
};

static struct irRx irRxVcrPause = {
   sizeof(vcrPause)/sizeof(char *),
   vcrPause,
   SZ_IR_VCR_PAUSE
};

static struct irRx irRxVcrPlay = {
   sizeof(vcrPlay)/sizeof(char *),
   vcrPlay,
   SZ_IR_VCR_PLAY
};

static struct irRx irRxVcrPower = {
   sizeof(vcrPower)/sizeof(char *),
   vcrPower,
   SZ_IR_VCR_POWER
};

static struct irRx irRxVcrRewind = {
   sizeof(vcrRewind)/sizeof(char *),
   vcrRewind,
   SZ_IR_VCR_REWIND
};

static struct irRx irRxVcrStop = {
   sizeof(vcrStop)/sizeof(char *),
   vcrStop,
   SZ_IR_VCR_STOP
};

static struct irRx irRxVcrVideo = {
   sizeof(vcrVideo)/sizeof(char *),
   vcrVideo,
   SZ_IR_VCR_VIDEO
};

static struct irRx irRxVolumeDown = {
   sizeof(volumeDown)/sizeof(char *),
   volumeDown,
   SZ_IR_VOLUME_DOWN
};

static struct irRx irRxVolumeUp = {
   sizeof(volumeUp)/sizeof(char *),
   volumeUp,
   SZ_IR_VOLUME_UP
};
   
