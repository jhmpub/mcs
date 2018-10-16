/**************************************************************************************************
  Filename:       rti.h
  Revised:        $Date: 2012-11-21 10:03:59 -0800 (Wed, 21 Nov 2012) $
  Revision:       $Revision: 32275 $

  Description:    This file contains the the RemoTI (RTI) API.

  Copyright 2008-2012 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

#ifndef RTI_H
#define RTI_H

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

// Hal Driver Includes
#include "hal_types.h"

// RTI Includes
#include "rti_constants.h"

// Remote Control Network layer attribute definition
#include "rcn_attribs.h"

/**************************************************************************************************
 * MACROS
 **************************************************************************************************/

// Define a prefix macro for exported functions and global variables
// which can be used for generating proper DLL interface for windows platform
#ifdef _WIN32
#  ifdef RTILIB_EXPORTS
#    define RTILIB_API __declspec(dllexport)
#  else
#    define RTILIB_API __declspec(dllimport)
#  endif
#else
#  define RTILIB_API
#endif

// Macro to build a byte for node capabilities - each parameter has to be set to either 0 or 1.
#define RTI_BUILD_NODE_CAPABILITIES(_target,_ac_powered,_security_capable,_ch_norm_capable ) \
  ((_target) | ((_ac_powered) << 1) | ((_security_capable) << 2) | ((_ch_norm_capable) << 3))

// Macro to build a byte for app capabilities - each parameter has to be set to either 0 or 1.
#define RTI_BUILD_APP_CAPABILITIES(_usr_str, _num_devs, _num_prof) \
  ((_usr_str) | ((_num_devs) << 1) | ((_num_prof) << 4))

#ifndef ATTR_PACKED
# ifdef __GNUC__
#  define ATTR_PACKED __attribute__ ((__packed__))
# else
#  define ATTR_PACKED
# endif
#endif

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

#if !defined FEATURE_USER_STRING_PAIRING
#define FEATURE_USER_STRING_PAIRING                      FALSE
#endif

// RTI API Status Values

// Application framework layer primitive status field values
#define RTI_SUCCESS                                      0
#define RTI_ERROR_INVALID_INDEX                          0xF9
#define RTI_ERROR_INVALID_PARAMETER                      0xE8
#define RTI_ERROR_UNSUPPORTED_ATTRIBUTE                  0xF4
#define RTI_ERROR_NO_ORG_CAPACITY                        0xB0
#define RTI_ERROR_NO_REC_CAPACITY                        0xB1
#define RTI_ERROR_NO_PAIRING_INDEX                       0xB2
#define RTI_ERROR_NO_RESPONSE                            0xB3
#define RTI_ERROR_NOT_PERMITTED                          0xB4
#define RTI_ERROR_FRAME_COUNTER_EXPIRED                  0xB6
#define RTI_ERROR_DISCOVERY_ERROR                        0xB7
#define RTI_ERROR_DISCOVERY_TIMEOUT                      0xB8
#define RTI_ERROR_SECURITY_TIMEOUT                       0xB9
#define RTI_ERROR_SECURITY_FAILURE                       0xBA
#define RTI_ERROR_NO_SECURITY_KEY                        0xBD
#define RTI_ERROR_OUT_OF_MEMORY                          0xBE
#define RTI_ERROR_OSAL_NO_TIMER_AVAIL                    0x08
#define RTI_ERROR_OSAL_NV_OPER_FAILED                    0x0A
#define RTI_ERROR_OSAL_NV_ITEM_UNINIT                    0x09
#define RTI_ERROR_OSAL_NV_BAD_ITEM_LEN                   0x0C
#define RTI_ERROR_MAC_TRANSACTION_EXPIRED                0xF0
#define RTI_ERROR_MAC_TRANSACTION_OVERFLOW               0xF1
#define RTI_ERROR_MAC_NO_RESOURCES                       0x1A
#define RTI_ERROR_MAC_UNSUPPORTED                        0x18
#define RTI_ERROR_MAC_BAD_STATE                          0x19
#define RTI_ERROR_MAC_CHANNEL_ACCESS_FAILURE             0xE1
#define RTI_ERROR_MAC_NO_ACK                             0xE9
#define RTI_ERROR_MAC_BEACON_LOST                        0xE0
#define RTI_ERROR_MAC_PAN_ID_CONFLICT                    0xEE
#define RTI_ERROR_MAC_SCAN_IN_PROGRESS                   0xFC
#define RTI_ERROR_UNKNOWN_STATUS_RETURNED                0x20
#define RTI_ERROR_FAILED_TO_DISCOVER                     0x21
#define RTI_ERROR_FAILED_TO_PAIR                         0x22
#define RTI_ERROR_ALLOW_PAIRING_TIMEOUT                  0x23
#define RTI_ERROR_FAILED_TO_CONFIGURE_ZRC                0x50
#define RTI_ERROR_FAILED_TO_CONFIGURE_ZID                0x51
#define RTI_ERROR_FAILED_TO_CONFIGURE_Z3D                0x52
#define RTI_ERROR_FAILED_TO_CONFIGURE_INV_MASK           0x50
  // reserve failure IDs 0x44-0x4A for future profiles
#define RTI_ERROR_SYNCHRONOUS_NPI_TIMEOUT                0xFF



// RTI API Parameter Constants

// RTI_SendDataReq TX Options Bit Mask
#define RTI_TX_OPTION_BROADCAST                          0x01
#define RTI_TX_OPTION_IEEE_ADDRESS                       0x02
#define RTI_TX_OPTION_ACKNOWLEDGED                       0x04
#define RTI_TX_OPTION_SECURITY                           0x08
#define RTI_TX_OPTION_SINGLE_CHANNEL                     0x10
#define RTI_TX_OPTION_CHANNEL_DESIGNATOR                 0x20
#define RTI_TX_OPTION_VENDOR_SPECIFIC                    0x40

// RTI_ReceiveDataInd RX Flags Bit Mask
#define RTI_RX_FLAGS_BROADCAST                           0x01
#define RTI_RX_FLAGS_SECURITY                            0x02
#define RTI_RX_FLAGS_VENDOR_SPECIFIC                     0x04

// RTI_RxEnableReq
#define RTI_RX_ENABLE_OFF                                0
#define RTI_RX_ENABLE_ON                                 0xFFFF

// RTI_StandbyReq
#define RTI_STANDBY_OFF                                  0
#define RTI_STANDBY_ON                                   1

// RTI Configuration Parameter default values
// The following configuration parameter default values are set arbitrarily.
// Modifying these constant values will require rebuilding RTI module.

#if RCN_FEATURE_SECURITY
#define RTI_DEFAULT_NODE_CAPABILITIES      (RTI_NODE_CAP_SEC_CAP_BM | RTI_NODE_CAP_CHAN_NORM_BM)
#else
#define RTI_DEFAULT_NODE_CAPABILITIES       RTI_NODE_CAP_CHAN_NORM_BM
#endif

#if FEATURE_USER_STRING_PAIRING
#define RTI_DEFAULT_APP_CAPABILITIES        RTI_APP_CAP_USER_STR_BM
#else
#define RTI_DEFAULT_APP_CAPABILITIES        0
#endif

#define RTI_STANDBY_ACTIVE_PERIOD                        16    // 16ms   (0x60 - nwkActivePeriod)
#define RTI_STANDBY_DUTY_CYCLE                           330   // 330ms  (0x64 - nwkDutyCycle)

// The follwoing constant values are dictated by
// RF4CE standard and hence cannot be modified at all.
#define RTI_MAX_NUM_DEV_TYPES                            RCN_MAX_NUM_DEV_TYPES
#define RTI_MAX_NUM_PROFILE_IDS                          RCN_MAX_NUM_PROFILE_IDS
#define RTI_VENDOR_STRING_LENGTH                         RCN_VENDOR_STRING_LENGTH
#define RTI_USER_STRING_LENGTH                           RCN_USER_STRING_LENGTH
#define RTI_DEST_PAN_ID_WILDCARD                         0xFFFF
#define RTI_DEST_NWK_ADDR_WILDCARD                       0xFFFF
#define RTI_REC_DEV_TYPE_WILDCARD                        0xFF
#define RTI_INVALID_PAIRING_REF                          0xFF
#define RTI_INVALID_DEVICE_TYPE                          0xFF

/**************************************************************************************************
 * ZRC specifies the following initial NIB attribute settings to comply with the proscribed
 * "Discovery/Pairing Procedure".
 */

// Controller discovery duration shall be 0x00186a symbols --> 100 msec.
#define DPP_DISCOVERY_DURATION             100

// Target automatic discovery response mode duration shall be 0x1c9c38 symbols --> 30 seconds.
#define DPP_AUTO_RESP_DURATION             30000

// 0x00f424 symbols --> 1 second.
#define DPP_DISCOVERY_REP_INTERVAL         1000   // 0x63 - nwkDiscoveryRepetitionInterval

#define DPP_MAX_DISCOVERY_REPS             30     // 0x69 - nwkDiscoveryRepetitions

#define DPP_MAX_REPORTED_NODE_DESC         1      // 0x6c - nwkMaxReportedNodeDescriptors

/**************************************************************************************************
 * ZRC leaves the following initial NIB attribute settings as implementation specific for the
 * "Discovery/Pairing Procedure".
 */

#define DPP_LQI_THRESHOLD                  0      // 0x62 - nwkDiscoveryLQIThreshold

/**************************************************************************************************
 * ZRC specifies the following constants.
 */

// Max time between consecutive user control repeated command frame transmissions.
#define aplcMaxKeyRepeatInterval           100   // Time in msec.

// Max duration that the receiver is enabled on a Controller after pairing to Rx a cmd discovery.
#define aplcMaxCmdDiscRxOnDuration         200   // Time in msec.

// Max time a Target waits after NLME-AUTO-DISCOVERY.confirm for a pair indication to arrive.
#define aplcZrcMaxPairIndicationWaitTime   1000   // Time in msec.

// Max time a device shall wait for a response command frame following a request command frame.
#define aplcMaxResponseWaitTime            200   // Time in msec.

// Min value of the KeyExTransferCount parameter passed to a pair request primitive during pairing.
#define aplcMinKeyExchangeTransferCount    3

// Min time a Controller must wait after pairing before attempting command discovery to the Target.
#define aplcMinTargetBlackoutPeriod        500   // Time in msec.

/**************************************************************************************************
 * ZRC specifies the following profile attributes.
 */

// Interval in msec at which user cmd repeat frames will be transmitted.
// The valid range is {0, aplcMaxKeyRepeatInterval}, default is (aplcMaxKeyRepeatInterval / 2).
// Currently, this attribute must be handled and acted upon at the ZRC Application layer.
#define aplKeyRepeatInterval               0x80

// Duration to wait after Rx of user control repeated cmd frame before terminating repeat operation.
// The valid range is >= (aplcMaxKeyRepeatInterval * 2), default is (aplcMaxKeyRepeatInterval * 2).
// Currently, this attribute must be handled and acted upon at the ZRC Application layer.
#define aplKeyRepeatWaitTime               0x81

// Value of KeyExTransferCount parameter passed to pair req primitive during DPP.
#define aplKeyExchangeTransferCount        0x82

#define DPP_DEF_KEY_TRANSFER_COUNT         36


// RTI Configuration Interface Item Identifiers

// State Attributes (SA) Table Item Identifiers
#define RTI_SA_ITEM_START                                RCN_NIB_NWK_START                   // 0x60
#define RTI_SA_ITEM_STANDBY_ACTIVE_PERIOD                RCN_NIB_NWK_ACTIVE_PERIOD           // 0x60
#define RTI_SA_ITEM_CURRENT_CHANNEL                      RCN_NIB_NWK_BASE_CHANNEL            // 0x61
#define RTI_SA_ITEM_DISCOVERY_LQI_THRESHOLD              RCN_NIB_NWK_DISCOVERY_LQI_THRESHOLD // 0x62
#define RTI_SA_ITEM_DUTY_CYCLE                           RCN_NIB_NWK_DUTY_CYCLE              // 0x64
#define RTI_SA_ITEM_FRAME_COUNTER                        RCN_NIB_NWK_FRAME_COUNTER           // 0x65
#define RTI_SA_ITEM_IN_POWER_SAVE                        RCN_NIB_NWK_IN_POWER_SAVE           // 0x67
#define RTI_SA_ITEM_MAX_FIRST_ATTEMPT_CSMA_BACKOFFS      RCN_NIB_NWK_MAX_FIRST_ATTEMPT_CSMA_BACKOFFS
#define RTI_SA_ITEM_MAX_FIRST_ATTEMPT_FRAME_RETRIES      RCN_NIB_NWK_MAX_FIRST_ATTEMPT_FRAME_RETRIES
#define RTI_SA_ITEM_RESPONSE_WAIT_TIME                   RCN_NIB_NWK_RESPONSE_WAIT_TIME      // 0x6D
#define RTI_SA_ITEM_SCAN_DURATION                        RCN_NIB_NWK_SCAN_DURATION           // 0x6E
#define RTI_SA_ITEM_USER_STRING                          RCN_NIB_NWK_USER_STRING             // 0x6F
#define RTI_SA_ITEM_PAN_ID                               RCN_NIB_PAN_ID                      // 0x85
#define RTI_SA_ITEM_SHORT_ADDRESS                        RCN_NIB_SHORT_ADDRESS               // 0x86
#define RTI_SA_ITEM_AGILITY_ENABLE                       RCN_NIB_AGILITY_ENABLE              // 0x87
#define RTI_SA_ITEM_TRANSMIT_POWER                       RCN_NIB_TRANSMIT_POWER              // 0x88

// Configuration Parameters (CP) Table Item Identifiers
#define RTI_CP_ITEM_START                                0xA0
#define RTI_CP_ITEM_STARTUP_CTRL                         0xA0   // Startup Control
#define RTI_CP_ITEM_NODE_CAPABILITIES                    0xA1   // Node Capabilities
#define RTI_CP_ITEM_NODE_SUPPORTED_TGT_TYPES             0xA2   // Node Capability::Supported Target Types
#define RTI_CP_ITEM_APPL_CAPABILITIES                    0xA3   // Application Capability::User String Specified
#define RTI_CP_ITEM_APPL_DEV_TYPE_LIST                   0xA4   // Application Capability::Device Type List
#define RTI_CP_ITEM_APPL_PROFILE_ID_LIST                 0xA5   // Application Capability::Profile Id List
#define RTI_CP_ITEM_STDBY_DEFAULT_ACTIVE_PERIOD          0xA6   // Standby Information::Default Standby Active Period
#define RTI_CP_ITEM_STDBY_DEFAULT_DUTY_CYCLE             0xA7   // Standby Information::Default Standby Duty Cycle
#define RTI_CP_ITEM_VENDOR_ID                            0xA8   // Vendor Information::Vendor Id
#define RTI_CP_ITEM_VENDOR_NAME                          0xA9   // Vendor Information::Vendor Name
#define RTI_CP_ITEM_DISCOVERY_DURATION                   0xAA   // Discovery Information::Discovery Duration
#define RTI_CP_ITEM_DEFAULT_DISCOVERY_LQI_THRESHOLD      0xAB   // Discovery Information::Default Discovery LQI Threshold
#define RTI_CP_ITEM_END                                  0xAF
// Pairing Table
#define RTI_SA_ITEM_PT_NUMBER_OF_ACTIVE_ENTRIES          0xB0
#define RTI_SA_ITEM_PT_CURRENT_ENTRY_INDEX               0xB1
#define RTI_SA_ITEM_PT_CURRENT_ENTRY                     0xB2
#define RTI_SA_ITEM_PT_END                               0xB3

// Constants (CONST) Table Item Idenifiers
#define RTI_CONST_ITEM_START                             0xC0
#define RTI_CONST_ITEM_SW_VERSION                        0xC0
#define RTI_CONST_ITEM_MAX_PAIRING_TABLE_ENTRIES         0xC1
#define RTI_CONST_ITEM_NWK_PROTOCOL_IDENTIFIER           0xC2
#define RTI_CONST_ITEM_NWK_PROTOCOL_VERSION              0xC3
#define RTI_CONST_ITEM_END                               0xC4

#if defined FEATURE_OAD
#define RTI_CONST_ITEM_OAD_IMAGE_ID                      0xD0
#endif
#define RTI_CONST_ITEM_RNP_IMAGE_ID                      0xD1
#define DPP_CP_ITEM_KEY_TRANSFER_CNT                     0xD2

// Constants Table Constants
// SW version has format of 0b'xxxyyyzz' where xxx is major rev; yyy is minor rev; zz is incremental rev
#define RTI_CONST_SW_VERSION                             0x2D   // V1.3.1 (001 011 01)
#define RTI_CONST_NWK_PROTOCOL_IDENTIFIER                0xCE   // Network layer protocol id
#define RTI_CONST_NWK_PROTOCOL_VERSION                   0x10   // V1.0

// Test Interface
#define RTI_TEST_MODE_TX_RAW_CARRIER                     0
#define RTI_TEST_MODE_TX_RANDOM_DATA                     1
#define RTI_TEST_MODE_RX_AT_FREQ                         2

// Configuration Capacity
#define RTI_MAX_NUM_SUPPORTED_TGT_TYPES                  6

// Bit masks for the Application Capabilities field (3.3.1.5, Figure 18).
#define RTI_APP_CAP_USER_STR_BM                          0x01
#define RTI_APP_CAP_NUM_DEVS_BM                          0x06
#define RTI_APP_CAP_RES_BIT3_BM                          0x08
#define RTI_APP_CAP_NUM_PROF_BM                          0x70
#define RTI_APP_CAP_RES_BIT7_BM                          0x80

// Bit masks for the Node Capabilities field (3.4.2.4, Figure 26).
#define RTI_NODE_CAP_NODE_TYPE_BM                        0x01
#define RTI_NODE_CAP_MAINS_PWR_BM                        0x02
#define RTI_NODE_CAP_SEC_CAP_BM                          0x04
#define RTI_NODE_CAP_CHAN_NORM_BM                        0x08
#define RTI_NODE_CAP_RESERVED_BM                         0xF0

/**************************************************************************************************
 * TYPEDEFS
 **************************************************************************************************/

// To be compatible with MS native target
// declare pragma for structure packing
#ifdef _MSC_VER
#pragma pack(1)
#endif

typedef uint8 rStatus_t;

// Configuration Parameters Table types

// Startup Control
enum
{
  NIB_RESTORE_STATE,
  NIB_CLEAR_STATE,
  NIB_CLEAR_CONFIG_CLEAR_STATE
};

// Controller Node Information
PACK_1 typedef struct ATTR_PACKED
{
  uint8 supportedTgtTypes[ RTI_MAX_NUM_SUPPORTED_TGT_TYPES ];
} tgtTypeInfo_s;

// Node Information
PACK_1 typedef struct ATTR_PACKED
{
  tgtTypeInfo_s supportedTgtTypes;
} nodeCap_s;

// Application Information
PACK_1 typedef struct ATTR_PACKED
{
  uint8 appCapabilities;
  uint8 devTypeList[ RTI_MAX_NUM_DEV_TYPES ];
  uint8 profileIdList[ RTI_MAX_NUM_PROFILE_IDS ];
} rtiAppCap_t;

// Standby Information
PACK_1 typedef struct ATTR_PACKED
{
  uint16 standbyActivePeriod;
  uint16 standbyDutyCycle;
} stdByInfo_s;

// Vendor Id
// <please see rti_constants.h>

// Vendor Information
PACK_1 typedef struct ATTR_PACKED
{
  uint16 vendorId;
  uint8  vendorName[ RTI_VENDOR_STRING_LENGTH ];
} vendorInfo_s;

PACK_1 typedef struct ATTR_PACKED
{
  uint16 discDuration; // in miliseconds
  uint8  discLQIThres;
} discInfo_s;

// Configuration Parameters Table
PACK_1 typedef struct ATTR_PACKED
{
  uint8        startupCtrl;                              // RTI_NVID_STARTUP_CONTROL
  nodeCap_s    nodeCap;                                  // RTI_NVID_NODE_CAP
  rtiAppCap_t  appCap;                                   // RTI_NVID_APP_CAP
  stdByInfo_s  stdByInfo;                                // RTI_NVID_STDBY_INFO
  discInfo_s   discoveryInfo;                            // RTI_NVID_DISCOV_INFO
  uint8        nodeCapabilities;
  vendorInfo_s vendorInfo;
} configParams_s;

// State Attributes Table
PACK_1 typedef struct ATTR_PACKED
{
  uint8  curPairTableIndex;
} stateAttribs_s;

// End the packing rule
#ifdef _MSC_VER
#pragma pack()
#endif


// function pointer for RCN event callback function
typedef void (*rtiRcnCbackFn_t)( void *pData );

/**************************************************************************************************
 * GLOBALS
 **************************************************************************************************/

extern uint8 RTI_TaskId;                 // Task ID
extern configParams_s configParamTable;  // Configuration Parameter Table

/*********************************************************************
 * FUNCTIONS
 */

/* These functions are used when creating the OSAL RTI task.  They must not be used for any
 * other purpose.
 */
extern void RTI_Init( uint8 task_id );
extern uint16 RTI_ProcessEvent( uint8 task_id, uint16 events );

// RF4CE Network Processor RemoTI (RTI) API

// Configuration Interface
// Used to access Configuration Parameters, State Attributes, and Constants
extern RTILIB_API rStatus_t RTI_ReadItemEx(uint8 profileId, uint8 itemId, uint8 len, uint8 *pValue);
extern RTILIB_API rStatus_t RTI_WriteItemEx(uint8 profileId, uint8 itemId, uint8 len, uint8 *pValue);

// Application Profile Interface
// Used to access RF4CE application profile
extern RTILIB_API void RTI_InitReq( void );
extern RTILIB_API void RTI_PairReq( void );
extern RTILIB_API void RTI_PairAbortReq( void );
extern RTILIB_API void RTI_UnpairReq( uint8 dstIndex );
extern RTILIB_API void RTI_AllowPairReq( void );
extern RTILIB_API void RTI_AllowPairAbortReq( void );
extern RTILIB_API void RTI_SendDataReq( uint8 dstIndex, uint8 profileId, uint16 vendorId, uint8 txOptions, uint8 len, uint8 *pData );
extern RTILIB_API void RTI_StandbyReq( uint8 mode );
extern RTILIB_API void RTI_RxEnableReq( uint16 duration );
extern RTILIB_API void RTI_EnableSleepReq( void );
extern RTILIB_API void RTI_DisableSleepReq( void );
extern RTILIB_API void RTI_EnterBootModeReq( void );

// RTI Callbacks
extern void RTI_InitCnf( rStatus_t status );
extern void RTI_PairCnf( rStatus_t status, uint8 dstIndex, uint8 devType );
extern void RTI_PairAbortCnf( rStatus_t status );
extern void RTI_UnpairCnf( rStatus_t status, uint8 dstIndex );
extern void RTI_UnpairInd( uint8 dstIndex );
extern void RTI_AllowPairCnf( rStatus_t status, uint8 dstIndex, uint8 devType );
extern void RTI_SendDataCnf( rStatus_t status );
extern void RTI_StandbyCnf( rStatus_t status );
extern void RTI_ReceiveDataInd( uint8 srcIndex, uint8 profileId, uint16 vendorId, uint8 rxLQI, uint8 rxFlags, uint8 len, uint8 *pData );
extern void RTI_RxEnableCnf( rStatus_t status );
extern void RTI_EnableSleepCnf( rStatus_t status );
extern void RTI_DisableSleepCnf( rStatus_t status );
extern void RTI_ResetInd( void );

// The following function is used by a module within radio processor.
// The functionsi not intended for use by application in host processor.
extern void RTI_SetBridgeMode(rtiRcnCbackFn_t pCback);


// It is better to compile flag RTI surrogate specific APIs
// with "RNP_HOST" as eventually there could be an SOC built on
// high level OS such as linux.
// For now, to maintain backward compatibility, no such compile
// flag is used.
#ifdef _WIN32
// This function is for windows platform only
 extern uint8 RTI_InitWin32Module(const char *pPortName);
 extern uint8 RTI_CloseWin32Module(void);
# define RTI_InitRNP(_pPortName) RTI_InitWin32Module(_pPortName)
# define RTI_CloseRNP() RTI_CloseWin32Module()
#elif defined __unix__
 extern int RTIS_Init(const char *pPortName);
 extern void RTIS_Close(void);
# define RTI_InitRNP(_pPortName) RTIS_Init(_pPortName)
# define RTI_CloseRNP() RTIS_Close()
#else
 extern void RTIS_Init( void );
#endif

// Test Interface
// Used to access test modes in the RemoTI stack.
extern RTILIB_API void RTI_SwResetReq( void );
extern RTILIB_API void RTI_TestModeReq( uint8 mode, int8 txPower, uint8 channel );
extern RTILIB_API uint16 RTI_TestRxCounterGetReq(uint8 resetFlag);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* RTI_H */
