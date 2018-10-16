#ifndef ZRCC_H
#define ZRCC_H

#ifdef __CYGWIN__
#define _WIN32  // override __unix__ directive in rti.h
#endif
#include <rti.h>

#define RTI_VENDOR_COMCAST       0x109D
#define RTI_PROFILE_RF4CE_MSO    0xC0
#define RTI_PROFILE_C1           0xC1
#define DEFAULT_ZRC_COM_PORT     "COM4"

// macro to build a byte for app capabilities
#define RFHOST_BUILD_APP_CAPABILITIES(_num_prof, _num_devs, _usr_str) \
  ((_usr_str) | ((_num_devs) << 1) | ((_num_prof) << 4))

// macro to build a byte for node capabilities
#define RFHOST_BUILD_NODE_CAPABILITIES(_ch_norm_capable, _security_capable, _ac_powered, _target ) \
  ((_target) | ((_ac_powered) << 1) | ((_security_capable) << 2) | ((_ch_norm_capable) << 3))

// arrow characters
#define CHAR_ARROW_UP    0x18
#define CHAR_ARROW_DOWN  0x19
#define CHAR_ARROW_RIGHT 0x1a
#define CHAR_ARROW_LEFT  0x1b


// define zigbee remote control command descriptions
enum zrcID {
   ZRC_ZERO,
   ZRC_ONE,
   ZRC_TWO,
   ZRC_THREE,
   ZRC_FOUR,
   ZRC_FIVE,
   ZRC_SIX,
   ZRC_SEVEN,
   ZRC_EIGHT,
   ZRC_NINE,
   ZRC_GUIDE,
   ZRC_UP,
   ZRC_DOWN,
   ZRC_RIGHT,
   ZRC_LEFT,
   ZRC_OK,
   ZRC_EXIT_PAGE,
   ZRC_NEXT_FAVORITE,      
   ZRC_CHANNEL_UP,         
   ZRC_CHANNEL_DOWN,       
   ZRC_PREVIOUS_CHANNEL,   
   ZRC_DISPLAY_INFO,
   ZRC_PAGE_UP,            
   ZRC_PAGE_DOWN,          
   ZRC_PLAY,               
   ZRC_PAUSE,              
   ZRC_REWIND,             
   ZRC_FAST_FORWARD,       
   ZRC_FORWARD,            
   ZRC_BACKWARD,           
};
   

// associate zigbee remote control CERC command information
struct cercCmd {
   int id;
   char key;
   uint8 cerc;
   const char * sz;
}; 

extern struct cercCmd cercCmd[];

// associate zigbee remote control message information for sendDataReq
struct zrcMsg {
   uint8 * data;
   uint8 len;
   const char * sz;
};   

char isRemoteControlDevice(void);
char cercToKey(uint8);
char * uint8ArrayToSz(uint8 *, int, char *, int);
const char * rtiConfigIdToName(int);
const char * statusToMsg(int);
int zrcMenuLoop(void);
struct cercCmd * keyToCercCmd(char);
struct cercCmd * szToCercCmd(const char *);
uint8 findPairingEntry(void);
uint8 keyToCerc(char);
void commitPairingSTB(void);
void configRNP(void);
void displayMenu(void);
void initZigbeeRemoteControl(const char *);
void rfClose(void);
void wakeupRNP(void);
void writeItem(uint8, uint8, uint8 * pValue);

#if ((defined HOST_CTL) && (HOST_CTL == TRUE))
// List of supported target device types: maximum of 6
// RTI_DEVICE_SET_TOP_BOX(0x09)
uint8 tgtList[RTI_MAX_NUM_SUPPORTED_TGT_TYPES] =
{
  RTI_DEVICE_SET_TOP_BOX,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID
};

// List of implemented device types: maximum up to 3 device types.
// RTI_DEVICE_REMOTE_CONTROL(0x01)
uint8 devList[RTI_MAX_NUM_DEV_TYPES] =
{
  RTI_DEVICE_REMOTE_CONTROL,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID
};

// List of implemented profiles: maximum of 7
// RTI_PROFILE_RF4CE_MSO(0xC0), RTI_PROFILE_C1(0xC1)
uint8 profileList[RTI_MAX_NUM_PROFILE_IDS] = {
//    RTI_PROFILE_RF4CE_MSO, RTI_PROFILE_C1, 0, 0, 0, 0, 0
    RTI_PROFILE_RF4CE_MSO, 0, 0, 0, 0, 0, 0
};

// 2 profiles (RF4CE MSO or C1), 1 device type (remote control), user string pairing
//uint8 appCapabilities = RTI_BUILD_APP_CAPABILITIES(2, 1, 1);  // 0x33 encoded
// 1 profile (RF4CE MSO), 1 device type (remote control), user string pairing
uint8 appCapabilities = RTI_BUILD_APP_CAPABILITIES(1, 1, 1);   // 0x13 encoded
      
// channel normalization capable, security capable, a/c power, target type
uint8 nodeCapabilities = RFHOST_BUILD_NODE_CAPABILITIES(1, 1, 0, 0);  // 0x0c encoded

uint8 userString[RTI_USER_STRING_LENGTH+1] = "XR11-20";

char isRemoteControlDevice() { return TRUE; }

#elif ((defined HOST_TGT) && (HOST_TGT == TRUE))
// List of supported target device types: maximum of 6
// RTI_DEVICE_REMOTE_CONTROL(0x01)
uint8 tgtList[RTI_MAX_NUM_SUPPORTED_TGT_TYPES] =
{
  RTI_DEVICE_REMOTE_CONTROL,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID
};

// List of implemented device types: maximum up to 3 device types.
// RTI_DEVICE_SET_TOP_BOX(0x09)
uint8 devList[RTI_MAX_NUM_DEV_TYPES] =
{
  RTI_DEVICE_SET_TOP_BOX,
  RTI_DEVICE_RESERVED_INVALID,
  RTI_DEVICE_RESERVED_INVALID
};

// List of implemented profiles: maximum of 7
// RTI_PROFILE_RF4CE_MSO(0xC0)
uint8 profileList[RTI_MAX_NUM_PROFILE_IDS] = {
    RTI_PROFILE_RF4CE_MSO, 0, 0, 0, 0, 0, 0
};

// 1 profile (RF4CE MSO), 2 device types (set top box or ?), user string pairing
uint8 appCapabilities = RTI_BUILD_APP_CAPABILITIES(1, 2, 1);   // 0x15 encoded
      
// channel normalization capable, security capable, a/c power, target type
uint8 nodeCapabilities = RFHOST_BUILD_NODE_CAPABILITIES(1, 1, 1, 1);   // 0x0f encoded

uint8 userString[RTI_USER_STRING_LENGTH+1] = "COMCAST";

char isRemoteControlDevice() { return FALSE; }

#elif ((defined HOST_CMN) && (HOST_CMN == TRUE))
extern uint8 tgtList[RTI_MAX_NUM_SUPPORTED_TGT_TYPES];
extern uint8 devList[RTI_MAX_NUM_DEV_TYPES];
extern uint8 profileList[RTI_MAX_NUM_PROFILE_IDS];
extern uint8 appCapabilities;
extern uint8 nodeCapabilities;
extern uint16 vendorId;
extern uint8 userString[RTI_USER_STRING_LENGTH+1];
#else
#error Must define module as Target, Controller or Common
#endif


extern char errMsg[256];
extern uint8 isSleeping;      // flag used to leave uart awake
extern uint8 pairingIndex;      // current target device destination index
extern uint8 zrcExiting;
extern uint8 zrcInitialized;
extern uint8 vendorName[RTI_VENDOR_STRING_LENGTH+1];
extern uint16 vendorId;

#endif  // ifndef ZRCC_H

