/**************************************************************************************************
  Filename:       rcn_attribs.h
  Revised:        $Date: 2011-03-22 21:23:52 -0700 (Tue, 22 Mar 2011) $
  Revision:       $Revision: 25490 $

  Description:    This file contains network layer attribute constants and type definitions.
                  The file can be included in upper layer interface definition without dragging in
                  other complex network layer interfaces.


  Copyright 2008-2011 Texas Instruments Incorporated. All rights reserved.

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

#ifndef RCN_ATTRIBS_H
#define RCN_ATTRIBS_H

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "hal_types.h"
#include "saddr.h"


/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

// Network Information Base (NIB) attribute identifier enumeration
#define RCN_NIB_NWK_START                         0x60 // enumeration starts
  // NOTE: active period NIB attribute is in miliseconds and it support 16 bit value
#define RCN_NIB_NWK_ACTIVE_PERIOD                 0x60 // nwkActivePeriod
#define RCN_NIB_NWK_BASE_CHANNEL                  0x61 // nwkBaseChannel
#define RCN_NIB_NWK_DISCOVERY_LQI_THRESHOLD       0x62 // nwkDiscoveryLQIThreshold
  // NOTE: discovery repetition internal NIB attribute is in miliseconds and supports 16 bit value
#define RCN_NIB_NWK_DISCOVERY_REPETITION_INTERVAL 0x63 // nwkDiscoveryRepetitionInterval
  // NOTE: duty cycle NIB attribute is in miliseconds and it supports 16 bit value
#define RCN_NIB_NWK_DUTY_CYCLE                    0x64 // nwkDutyCycle
#define RCN_NIB_NWK_FRAME_COUNTER                 0x65 // nwkFrameCounter
#define RCN_NIB_NWK_INDICATE_DISCOVERY_REQUESTS   0x66 // nwkIndicateDiscoveryRequests
#define RCN_NIB_NWK_IN_POWER_SAVE                 0x67 // nwkInPowerSave
#define RCN_NIB_NWK_PAIRING_TABLE                 0x68 // nwkPairingTable
#define RCN_NIB_NWK_MAX_DISCOVERY_REPETITIONS     0x69 // nwkMaxDiscoveryRepetitions
#define RCN_NIB_NWK_MAX_FIRST_ATTEMPT_CSMA_BACKOFFS 0x6a // nwkMaxFirstAttemptCSMABackoffs
#define RCN_NIB_NWK_MAX_FIRST_ATTEMPT_FRAME_RETRIES 0x6b // nwkMaxFirstAttemptFrameRetries
#define RCN_NIB_NWK_MAX_REPORTED_NODE_DESCRIPTORS 0x6c // nwkMaxReportedNodeDescriptors
  // NOTE: response wait time attribute is in miliseconds and it supports 16 bit value
#define RCN_NIB_NWK_RESPONSE_WAIT_TIME            0x6d // nwkResponseWaitTime
#define RCN_NIB_NWK_SCAN_DURATION                 0x6e // nwkScanDuration
#define RCN_NIB_NWK_USER_STRING                   0x6f // nwkUserString
#define RCN_NIB_NWK_RESERVED_0                    0x70 // end of standard attribute enumeration

// Proprietary attributes (these are not RF4CE NIB attribute but are used anyways)
#define RCN_NIB_CUSTOM_START                      0x80 // start of custom attribute enumeration
#define RCN_NIB_NWK_NODE_CAPABILITIES             0x80 // nwkcNodeCapabilities - constant in standard
#define RCN_NIB_NWK_VENDOR_IDENTIFIER             0x81 // nwkcVendorIdentifier - constant in standard
#define RCN_NIB_NWK_VENDOR_STRING                 0x82 // nwkcVendorString - constant in standard
#define RCN_NIB_STARTED                           0x83 // an indicator whether the node has
                                                       // started or not
#define RCN_NIB_IEEE_ADDRESS                      0x84 // IEEE address of the device
#define RCN_NIB_PAN_ID                            0x85 // target node PAN ID
#define RCN_NIB_SHORT_ADDRESS                     0x86 // target node short address
#define RCN_NIB_AGILITY_ENABLE                    0x87 // flag set to TRUE/FALSE for frequency agility enable
#define RCN_NIB_TRANSMIT_POWER                    0x88 // transmit power, uint8 value * -1dBm

// proprietary debug attributes
#define RCN_NIB_TRANSMIT_LATENCY                  0x89 // latency of last transmission
#define RCN_NIB_ENERGY_SAMPLE                     0x8a // energy scan samples
#define RCN_NIB_CUSTOM_RESERVED_0                 0x8b // end of custom attribute enumeration

#define RCN_NIB_NWK_RANGE_END                     0x8f // end of NIB attribute range

// Network layer implicit constants as in standard used by attribute structure
#define RCN_MAX_NUM_DEV_TYPES                     3 // number of device types supported per device
#define RCN_SEC_KEY_LENGTH                        16 // length in bytes of network layer security key
#define RCN_MAX_ATTRIB_SIZE_EXCEPT_PAIR_TBL       8 // maximum attribute value size in bytes except
                                                    // pairing table entry
#define RCN_MAX_NUM_PROFILE_IDS                   7 // number of profile IDs supported per device
#define RCN_VENDOR_STRING_LENGTH                  7
#define RCN_USER_STRING_LENGTH                    15
#define RCN_SEC_KEY_SEED_LENGTH                   80

// Security feature compile flag.
// Disabling this feature reduces the pairing table size but
// note that feature flag definitioin has to match the RCN library
// that the application is compiled with.
// That is, both application code and the RCN library must have been compiled with
// the same feature selection.
#ifndef RCN_FEATURE_SECURITY
#define RCN_FEATURE_SECURITY                      1
#endif

// Extra pairing entry information feature compile flag.
// Extra pairing entry information pertains to vendor ID and device type list.
// Disabling this feature reduces the pairing table size but
// note that feature flag definitioin has to match the RCN library
// that the application is compiled with.
// That is, both application code and the RCN library must have been compiled with
// the same feature selection.
#ifndef RCN_FEATURE_EXTRA_PAIR_INFO
#define RCN_FEATURE_EXTRA_PAIR_INFO               TRUE
#endif

#if RCN_FEATURE_EXTRA_PAIR_INFO
// Discrete bit definitions for rcnNwkPairingEntry_t.profileDiscs[].
#define RCN_PROFILE_DISC_GT8                      0  // GT8 set would imply one or more >= 8.
#define RCN_PROFILE_DISC_ZRC                      1
#define RCN_PROFILE_DISC_ZID                      2
#define RCN_PROFILE_DISC_Z3D                      3
#define RCN_PROFILE_DISC_SP4                      4
#define RCN_PROFILE_DISC_SP5                      5
#define RCN_PROFILE_DISC_SP6                      6
#define RCN_PROFILE_DISC_SP7                      7

#define RCN_PROFILE_DISCS_SIZE                    1  // Profile Discretes bytes allowed.
#endif

#if !defined PACK_1
#define PACK_1
#endif

/**************************************************************************************************
 * TYPEDEFS
 **************************************************************************************************/

#if defined(_MSC_VER) || (defined(__ICC430__) && (__ICC430__==1))
#pragma pack(1)
#endif

// Pairing table entry
PACK_1 typedef struct
{
  uint8      pairingRef;     // pairing reference
  uint16     srcNwkAddress; // this device's own short address
  uint8      logicalChannel; // expected channel of the peer
  sAddrExt_t ieeeAddress;    // IEEE address of the peer
  uint16     panId;          // PAN identifier of the peer
  uint16     nwkAddress;   // network address of the peer
  uint8      recCapabilities; // capabilities bitmap of the target

  uint8      securityKeyValid; // whether the key is valid or not
  uint8      securityKey[RCN_SEC_KEY_LENGTH]; // security link key

#if RCN_FEATURE_EXTRA_PAIR_INFO
  // NOTE that RF4CE spec does not include the following as part of pairing entry
  uint16     vendorIdentifier; // vendor identifier of the target
  uint8      devTypeList[RCN_MAX_NUM_DEV_TYPES]; // list of device types supported by the peer
#endif

  // The pairing table entry up to here is stored in NV items RCN_NVID_PAIRING_ENTRY_0 through
  // (RCN_NVID_PAIRING_ENTRY_0 + RCN_CAP_MAX_PAIRS - 1).

  // The recFrameCounter is stored in NV items (RCN_NVID_PAIRING_ENTRY_0 + RCN_CAP_MAX_PAIRS)
  // through (RCN_NVID_PAIRING_ENTRY_0 + RCN_CAP_MAX_PAIRS X 2 - 1).
  uint32     recFrameCounter;    // frame counter last received

#if RCN_FEATURE_EXTRA_PAIR_INFO
  // NOTE that RF4CE spec does not include the following as part of pairing entry

  // The profileDiscs are collected in an array of size RCN_CAP_MAX_PAIRS and stored as NV item
  // (RCN_NVID_PAIRING_ENTRY_0 + RCN_CAP_MAX_PAIRS X 2).
  uint8      profileDiscs[RCN_PROFILE_DISCS_SIZE];
#endif
} rcnNwkPairingEntry_t;

#ifdef _MSC_VER
#pragma pack()
#endif

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* RCN_ATTRIBS_H */
