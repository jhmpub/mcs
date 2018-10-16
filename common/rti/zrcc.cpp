//
//   zrcc.cpp (zigbee remote control common)
//
//   Functions in this module are common to both target and controller 
//   applications.
//
//   2018 Jun 12 jhm original creation 

#include <stdio.h>
#include <windows.h>
#include "extern.h"

// direct zrcc.h to not compile this module as a controller or a target
#define HOST_CMN         TRUE
#include "zrc.h"
#include "zrcc.h"

uint8 vendorName[RTI_VENDOR_STRING_LENGTH+1] = "COMCAST";
uint16 vendorId = RTI_VENDOR_COMCAST;
uint8 pairingIndex = RTI_INVALID_PAIRING_REF; // current target device destination index
rcnNwkPairingEntry_t apPairingEntryBuf;     // pairing reference entry buffer
struct zrcMsg zrcMsg;    // zigbee remote control message


// zigbee Consumer Electronics Remote Control (CERC) commands
struct cercCmd cercCmd[] = {
   {ZRC_ZERO,                '0',               RTI_CERC_NUM_0,                    SZ_0},
   {ZRC_ONE,                 '1',               RTI_CERC_NUM_1,                    SZ_1},
   {ZRC_TWO,                 '2',               RTI_CERC_NUM_2,                    SZ_2},
   {ZRC_THREE,               '3',               RTI_CERC_NUM_3,                    SZ_3},
   {ZRC_FOUR,                '4',               RTI_CERC_NUM_4,                    SZ_4},
   {ZRC_FIVE,                '5',               RTI_CERC_NUM_5,                    SZ_5},
   {ZRC_SIX,                 '6',               RTI_CERC_NUM_6,                    SZ_6},
   {ZRC_SEVEN,               '7',               RTI_CERC_NUM_7,                    SZ_7},
   {ZRC_EIGHT,               '8',               RTI_CERC_NUM_8,                    SZ_8},
   {ZRC_NINE,                '9',               RTI_CERC_NUM_9,                    SZ_9},
   {ZRC_GUIDE,               'g',               RTI_CERC_ELECTRONIC_PROGRAM_GUIDE, SZ_GUIDE},
   {ZRC_UP,                  CHAR_ARROW_UP,     RTI_CERC_UP,                       SZ_ARROW_UP},
   {ZRC_DOWN,                CHAR_ARROW_DOWN,   RTI_CERC_DOWN,                     SZ_ARROW_DOWN},
   {ZRC_RIGHT,               CHAR_ARROW_RIGHT,  RTI_CERC_RIGHT,                    SZ_ARROW_RIGHT},
   {ZRC_LEFT,                CHAR_ARROW_LEFT,   RTI_CERC_LEFT,                     SZ_ARROW_LEFT},
   {ZRC_OK,                  'o',               RTI_CERC_SELECT,                   SZ_OK},
   {ZRC_EXIT_PAGE,           'x',               RTI_CERC_EXIT,                     SZ_STOP},
   {ZRC_NEXT_FAVORITE,       'n',               RTI_CERC_NEXT_FAVORITE,            SZ_NEXT_FAVORITE},
   {ZRC_CHANNEL_UP,          '+',               RTI_CERC_CHANNEL_UP,               SZ_CHANNEL_UP},
   {ZRC_CHANNEL_DOWN,        '-',               RTI_CERC_CHANNEL_DOWN,             SZ_CHANNEL_DOWN},
   {ZRC_PREVIOUS_CHANNEL,    'l',               RTI_CERC_PREVIOUS_CHANNEL,         SZ_PREVIOUS_CHANNEL},
   {ZRC_DISPLAY_INFO,        'i',               RTI_CERC_DISPLAY_INFORMATION,      SZ_DISPLAY_INFO},
   {ZRC_PAGE_UP,             '^',               RTI_CERC_PAGE_UP,                  SZ_PAGE_UP},
   {ZRC_PAGE_DOWN,           '_',               RTI_CERC_PAGE_DOWN,                SZ_PAGE_DOWN},
   {ZRC_PLAY,                '>',               RTI_CERC_PAUSE_PLAY_FUNCTION,      SZ_PLAY},
   {ZRC_PAUSE,               '"',               RTI_CERC_PAUSE_PLAY_FUNCTION,      SZ_PAUSE},
   {ZRC_REWIND,              '[',               RTI_CERC_REWIND,                   SZ_REWIND},
   {ZRC_FAST_FORWARD,        ']',               RTI_CERC_FAST_FORWARD,             SZ_FAST_FORWARD},
   {ZRC_FORWARD,             '}',               RTI_CERC_FORWARD,                  "forward"},
   {ZRC_BACKWARD,            '{',               RTI_CERC_BACKWARD,                 "backward"},
};


// associate api status codes with meaningful error messages
struct statusMsg {
   int status;
   const char * msg;
} statusMsg[] = {
   {RTI_SUCCESS,                            "RTI_SUCCESS"},   
   {RTI_ERROR_INVALID_INDEX,                "RTI_ERROR_INVALID_INDEX"},   
   {RTI_ERROR_INVALID_PARAMETER,            "RTI_ERROR_INVALID_PARAMETER"},   
   {RTI_ERROR_UNSUPPORTED_ATTRIBUTE,        "RTI_ERROR_UNSUPPORTED_ATTRIBUTE"},   
   {RTI_ERROR_NO_ORG_CAPACITY,              "RTI_ERROR_NO_ORG_CAPACITY"},   
   {RTI_ERROR_NO_REC_CAPACITY,              "RTI_ERROR_NO_REC_CAPACITY"},   
   {RTI_ERROR_NO_PAIRING_INDEX,             "RTI_ERROR_NO_PAIRING_INDEX"},   
   {RTI_ERROR_NO_RESPONSE,                  "RTI_ERROR_NO_RESPONSE"},   
   {RTI_ERROR_NOT_PERMITTED,                "RTI_ERROR_NOT_PERMITTED"},   
   {RTI_ERROR_FRAME_COUNTER_EXPIRED,        "RTI_ERROR_FRAME_COUNTER_EXPIRED"},   
   {RTI_ERROR_DISCOVERY_ERROR,              "RTI_ERROR_DISCOVERY_ERROR"},   
   {RTI_ERROR_DISCOVERY_TIMEOUT,            "RTI_ERROR_DISCOVERY_TIMEOUT"},   
   {RTI_ERROR_SECURITY_TIMEOUT,             "RTI_ERROR_SECURITY_TIMEOUT"},   
   {RTI_ERROR_SECURITY_FAILURE,             "RTI_ERROR_SECURITY_FAILURE"},   
   {RTI_ERROR_NO_SECURITY_KEY,              "RTI_ERROR_NO_SECURITY_KEY"},   
   {RTI_ERROR_OUT_OF_MEMORY,                "RTI_ERROR_OUT_OF_MEMORY"},   
   {RTI_ERROR_ALLOW_PAIRING_TIMEOUT,        "RTI_ERROR_ALLOW_PAIRING_TIMEOUT"},   
   {RTI_ERROR_OSAL_NO_TIMER_AVAIL,          "RTI_ERROR_OSAL_NO_TIMER_AVAIL"},   
   {RTI_ERROR_OSAL_NV_OPER_FAILED,          "RTI_ERROR_OSAL_NV_OPER_FAILED"},   
   {RTI_ERROR_OSAL_NV_ITEM_UNINIT,          "RTI_ERROR_OSAL_NV_ITEM_UNINIT"},   
   {RTI_ERROR_OSAL_NV_BAD_ITEM_LEN,         "RTI_ERROR_OSAL_NV_BAD_ITEM_LEN"},   
   {RTI_ERROR_MAC_TRANSACTION_EXPIRED,      "RTI_ERROR_MAC_TRANSACTION_EXPIRED"},   
   {RTI_ERROR_MAC_TRANSACTION_OVERFLOW,     "RTI_ERROR_MAC_TRANSACTION_OVERFLOW"},   
   {RTI_ERROR_MAC_NO_RESOURCES,             "RTI_ERROR_MAC_NO_RESOURCES"},   
   {RTI_ERROR_MAC_BAD_STATE,                "RTI_ERROR_MAC_BAD_STATE"},   
   {RTI_ERROR_MAC_CHANNEL_ACCESS_FAILURE,   "RTI_ERROR_MAC_CHANNEL_ACCESS_FAILURE"},   
   {RTI_ERROR_MAC_NO_ACK,                   "RTI_ERROR_MAC_NO_ACK"},   
   {RTI_ERROR_MAC_BEACON_LOST,              "RTI_ERROR_MAC_BEACON_LOST"},   
   {RTI_ERROR_MAC_PAN_ID_CONFLICT,          "RTI_ERROR_MAC_PAN_ID_CONFLICT"},   
   {RTI_ERROR_MAC_SCAN_IN_PROGRESS,         "RTI_ERROR_MAC_SCAN_IN_PROGRESS"},   
   {RTI_ERROR_UNKNOWN_STATUS_RETURNED,      "RTI_ERROR_UNKNOWN_STATUS_RETURNED"},
   {RTI_ERROR_FAILED_TO_DISCOVER,           "RTI_ERROR_FAILED_TO_DISCOVER"},   
   {RTI_ERROR_FAILED_TO_PAIR,               "RTI_ERROR_FAILED_TO_PAIR"},   
   {RTI_ERROR_ALLOW_PAIRING_TIMEOUT,        "RTI_ERROR_ALLOW_PAIRING_TIMEOUT"},   
   {RTI_ERROR_FAILED_TO_CONFIGURE_ZRC,      "RTI_ERROR_FAILED_TO_CONFIGURE_ZRC"},   
   {RTI_ERROR_FAILED_TO_CONFIGURE_ZID,      "RTI_ERROR_FAILED_TO_CONFIGURE_ZID"},   
   {RTI_ERROR_FAILED_TO_CONFIGURE_Z3D,      "RTI_ERROR_FAILED_TO_CONFIGURE_Z3D"},   
   {RTI_ERROR_FAILED_TO_CONFIGURE_INV_MASK, "RTI_ERROR_FAILED_TO_CONFIGURE_INV_MASK"},
   {RTI_ERROR_SYNCHRONOUS_NPI_TIMEOUT,      "RTI_ERROR_SYNCHRONOUS_NPI_TIMEOUT"}
};


// associate remoti api configuration parameter #defines with descriptive text
struct rtiConfig {
   int id;
   const char * name;
} rtiConfig[] = {
   {RTI_CP_ITEM_NODE_CAPABILITIES,           "RTI_CP_ITEM_NODE_CAPABILITIES"},
   {RTI_CP_ITEM_NODE_SUPPORTED_TGT_TYPES,    "RTI_CP_ITEM_NODE_SUPPORTED_TGT_TYPES"},
   {RTI_CP_ITEM_APPL_CAPABILITIES,           "RTI_CP_ITEM_APPL_CAPABILITIES"},
   {RTI_CP_ITEM_APPL_DEV_TYPE_LIST,          "RTI_CP_ITEM_APPL_DEV_TYPE_LIST"},
   {RTI_CP_ITEM_APPL_PROFILE_ID_LIST,        "RTI_CP_ITEM_APPL_PROFILE_ID_LIST"},
   {RTI_CP_ITEM_STDBY_DEFAULT_ACTIVE_PERIOD, "RTI_CP_ITEM_STDBY_DEFAULT_ACTIVE_PERIOD"},
   {RTI_CP_ITEM_STDBY_DEFAULT_DUTY_CYCLE,    "RTI_CP_ITEM_STDBY_DEFAULT_DUTY_CYCLE"},
   {RTI_CP_ITEM_VENDOR_ID,                   "RTI_CP_ITEM_VENDOR_ID"},
   {RTI_CP_ITEM_VENDOR_NAME,                 "RTI_CP_ITEM_VENDOR_NAME"},
   {RTI_SA_ITEM_USER_STRING,                 "RTI_SA_ITEM_USER_STRING"},
};   

void RTI_AllowPairCnf( rStatus_t status, uint8 dstIndex, uint8 devType ) {}
void RTI_DiscoverCnf( rStatus_t status ) {}
void RTI_StandbyCnf( rStatus_t status ) {}
void RTI_ReceiveDataInd( uint8 srcIndex, uint8 profileId, uint16 vendorId, 
                         uint8 rxLQI, uint8 rxFlags, uint8 len, uint8 *pData ) {}
void RTI_RxEnableCnf( rStatus_t status ) {}
void RTI_EnableSleepCnf( rStatus_t status ) { isSleeping=TRUE; }
void RTI_UnpairInd( uint8 dstIndex ) {}
void RTI_PairAbortCnf( rStatus_t status ) {}
extern "C" void RCN_CbackEvent(void *pEvent) {}


const char * statusToMsg(int status) {
   int i;
   for (i=0; i<sizeof(statusMsg)/sizeof(struct statusMsg); i++) {
      if (statusMsg[i].status == status)
         return statusMsg[i].msg;
   }   
   return "UNDEFINED";
}   


uint8 keyToCerc(char key) {
   int i;
   for (i=0; i<sizeof(cercCmd)/sizeof(struct cercCmd); i++) {
      if (cercCmd[i].key == key)
         return cercCmd[i].cerc;
   }   
   return '\0';
}   

struct cercCmd * keyToCercCmd(char key) {
   int i;
   for (i=0; i<sizeof(cercCmd)/sizeof(struct cercCmd); i++) {
      if (cercCmd[i].key == key)
         return &cercCmd[i];
   }   
   return NULL;
}   

struct cercCmd * szToCercCmd(const char * sz) {
   int i;
   for (i=0; i<sizeof(cercCmd)/sizeof(struct cercCmd); i++) {
      if (!strcmp(cercCmd[i].sz, sz))
         return &cercCmd[i];
   }   
   return NULL;
}   

const char * rtiConfigIdToName(int id) {
   int i;
   for (i=0; i<sizeof(rtiConfig)/sizeof(struct rtiConfig); i++) {
      if (rtiConfig[i].id == id)
         return rtiConfig[i].name;
   }   
   return "UNDEFINED";
}   


char cercToKey(uint8 cerc) {
   int i;
   for (i=0; i<sizeof(cercCmd)/sizeof(struct cercCmd); i++) {
      if (cercCmd[i].cerc == cerc)
         return cercCmd[i].key;
   }   
   return '\0';
}


char * uint8ArrayToSz(uint8 * pData, int pDataLen, char * szData, int szDataLen) {
   int i, strCnt=0;
   const char * szFormat = "0x%02x ";
   char szChar[8];
   szData[0] = '\0';
   for (i=0; strlen(szData) < szDataLen && i < pDataLen; i++) {
      sprintf(szChar, szFormat, pData[i]);
      strncat(szData, szChar, szDataLen); 
   }
   return szData;
}      


// RTI_DisableSleepReq triggers an exception if the interface is uninitialized
uint8 zrcInitialized = FALSE;
void wakeupRNP(void) {
   if (zrcInitialized) {
      RTI_DisableSleepReq();
   }   
}


// write a parameter to the RNP usb dongle nv memory
void writeItem(uint8 itemId, uint8 length, uint8 * pValue) {
   char szErrMsg[256];
   if (RTI_SUCCESS != RTI_WriteItemEx(RTI_PROFILE_RTI, itemId, length, pValue)) {
      snprintf(szErrMsg, sizeof(szErrMsg), 
               "writeItem %s failed", rtiConfigIdToName(itemId));
      rfClose();
      errorExit(szErrMsg);
   }   
   if (!zrcInitialized)
      printf(".");
}


// store non-default configuration parameters to the Remoti Network Processor
// nv memory
void configRNP() {
   printf("Configuring Zigbee RNP");
   writeItem(RTI_CP_ITEM_NODE_SUPPORTED_TGT_TYPES, 
             RTI_MAX_NUM_SUPPORTED_TGT_TYPES, tgtList);
   writeItem(RTI_CP_ITEM_APPL_CAPABILITIES, 1, &appCapabilities);
   writeItem(RTI_CP_ITEM_APPL_DEV_TYPE_LIST, RTI_MAX_NUM_DEV_TYPES, devList);
   writeItem(RTI_CP_ITEM_APPL_PROFILE_ID_LIST, RTI_MAX_NUM_PROFILE_IDS, profileList);
   writeItem(RTI_CP_ITEM_NODE_CAPABILITIES, 1, &nodeCapabilities);
   writeItem(RTI_CP_ITEM_VENDOR_ID, 2, (uint8 *) &vendorId);
   writeItem(RTI_CP_ITEM_VENDOR_NAME, RTI_VENDOR_STRING_LENGTH, vendorName);
   writeItem(RTI_SA_ITEM_USER_STRING, RTI_USER_STRING_LENGTH, userString);
   printf("done\n");
}


uint8 findPairingEntry() {
   uint8 i, count;
   rcnNwkPairingEntry_t apPairingEntryBuf;   // pairing reference entry buffer
  
   printf("Reading RNP pairing entry table");

   // get the current pairing table index, if there is one
   pairingIndex = RTI_INVALID_PAIRING_REF;
   RTI_ReadItemEx(RTI_PROFILE_RTI, RTI_CONST_ITEM_MAX_PAIRING_TABLE_ENTRIES, 1, &count);
   printf(".");
   for (i = 0; i < count; i++) {
      RTI_WriteItemEx(RTI_PROFILE_RTI, RTI_SA_ITEM_PT_CURRENT_ENTRY_INDEX, 1, &i);
      printf(".");
      if (RTI_ReadItemEx(RTI_PROFILE_RTI, RTI_SA_ITEM_PT_CURRENT_ENTRY, 
                         sizeof(apPairingEntryBuf),
                         (uint8 *) &apPairingEntryBuf ) == RTI_SUCCESS) {
         // only succeeds if the current pairing table entry is valid 
         pairingIndex = i;  // use last entry in table if more than one pairing
      }
   }
   
   printf("done\n");
   
   printf("%s entry found\n", 
      (pairingIndex == RTI_INVALID_PAIRING_REF) ? "No pairing" : "Pairing");
      
   return pairingIndex;   
}


// keypress commands to pair, unpair, send data and quit
int zrcMenuLoop(void) {

   char menu_ch = '\0';

   displayMenu();
   tty_buffering(FALSE);
   
   while (menu_ch != 'q') {
   
      menu_ch = getChar();
      
      char asleep=isZrcCmdQueueEmpty();   
      
      switch (menu_ch) {
         case 'p':
            zrcMsg.sz = PAIR;
            zrcCmdQueueAdd(&zrcMsg);
            break;
         case 'u':
            zrcMsg.sz = UNPAIR;
            zrcCmdQueueAdd(&zrcMsg);
            break;
         case '?':    
            displayMenu();
         case 'q':
            continue;
         default:
            if (keyToCercCmd(menu_ch) && isRemoteControlDevice())
               zrcCmdQueueAdd(keyToCercCmd(menu_ch));
            else {
               printf("undefined cmd: 0x%02x\n", menu_ch);
               continue;
            }   
      }
      
      if (asleep)
         wakeupRNP();
   }
   
   rfClose();
   return 0;
}


void displayMenu() {
   int i;
   char szName[16];

   if (isRemoteControlDevice()) {
      // control devices send commands, target devices receive
      printf("\nCERC commands:\n");
      for (i=0; i<sizeof(cercCmd)/sizeof(struct cercCmd); i++)
         printf("%-2s   %s\n", 
                keyToName(cercCmd[i].key, szName, sizeof(szName)),
                cercCmd[i].sz);
   }
   printf("\n");   
   printf("p    pair\n");
   printf("u    unpair\n");
   printf("q    quit\n\n");
}


// initialize the Texas Instruments CC2531 Zigbee USB dongle
void initZigbeeRemoteControl(const char * comPort) {

   // load the Remoti Network Processor library and connect to the device
   if (!RTI_InitRNP(DEFAULT_ZRC_COM_PORT)) {
      const char * errFmt = "Error: failed to start RTI library module on %s";
      char * errMsg = (char *) malloc(strlen(errFmt) + 
                                      strlen(DEFAULT_ZRC_COM_PORT));
      snprintf(errMsg, 
               strlen(errFmt) + strlen(DEFAULT_ZRC_COM_PORT), 
               errFmt, DEFAULT_ZRC_COM_PORT);
      errorExit(errMsg);
   }

   // set non-default configuration parameters
   configRNP();  

   // initialize the remoti RF4CE stack
   printf("Calling RTI_InitReq...");
   RTI_InitReq(); 
   
   // wait for initialization confirmation from the RTI_InitCnf callback
   while (!zrcInitialized)
      Sleep(10);   
   
}


uint8 isSleeping = TRUE;   // assume sleeping at init, exit with uart awake
uint8 zrcExiting = FALSE;
void rfClose() {
   zrcExiting = TRUE;
   if (zrcInitialized) {
      isSleeping=TRUE;
      wakeupRNP();   // wake up RNP UART interface for next application run
      while(isSleeping) {
         // waiting for a wakeup is moot if the RNP is not initialized 
         Sleep(100);
      }
      RTI_CloseRNP();
      zrcInitialized = FALSE;      
   }
   tty_buffering(TRUE);
}


