/**************************************************************************************************
  Filename:       rti_constants.h
  Revised:        $Date: 2011-09-07 18:03:16 -0700 (Wed, 07 Sep 2011) $
  Revision:       $Revision: 27485 $

  Description:    This file contains RF4CE RemoTI constants.


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

#ifndef RTI_CONSTANTS_H
#define RTI_CONSTANTS_H

#ifdef __cplusplus
extern "C"
{
#endif

// Device Type List
#define RTI_DEVICE_RESERVED_INVALID                   0x00
#define RTI_DEVICE_REMOTE_CONTROL                     0x01
#define RTI_DEVICE_TARGET_TYPE_START                  0x02
#define RTI_DEVICE_TELEVISION                         0x02
#define RTI_DEVICE_PROJECTOR                          0x03
#define RTI_DEVICE_PLAYER                             0x04
#define RTI_DEVICE_RECORDER                           0x05
#define RTI_DEVICE_VIDEO_PLAYER_RECORDER              0x06
#define RTI_DEVICE_AUDIO_PLAYER_RECORDER              0x07
#define RTI_DEVICE_AUDIO_VIDEO_RECORDER               0x08
#define RTI_DEVICE_SET_TOP_BOX                        0x09
#define RTI_DEVICE_HOME_THEATER_SYSTEM                0x0A
#define RTI_DEVICE_MEDIA_CENTER_PC                    0x0B
#define RTI_DEVICE_GAME_CONSOLE                       0x0C
#define RTI_DEVICE_SATELLITE_RADIO_RECEIVER           0x0D
#define RTI_DEVICE_IR_EXTENDER                        0x0E
#define RTI_DEVICE_MONITOR                            0x0F
#define RTI_DEVICE_TARGET_TYPE_END                    0x10
// 0x10..0xFD: Reserved
#define RTI_DEVICE_GENERIC                            0xFE
#define RTI_DEVICE_RESERVED_FOR_WILDCARDS             0xFF

#define MAX_AVAIL_DEVICE_TYPES (RTI_DEVICE_TARGET_TYPE_END - RTI_DEVICE_TARGET_TYPE_START)

// vendor identifiers
#define RTI_VENDOR_PANASONIC                          0x0001
#define RTI_VENDOR_SONY                               0x0002
#define RTI_VENDOR_SAMSUNG                            0x0003
#define RTI_VENDOR_PHILIPS                            0x0004
#define RTI_VENDOR_FREESCALE                          0x0005
#define RTI_VENDOR_OKI                                0x0006
#define RTI_VENDOR_TEXAS_INSTRUMENTS                  0x0007

// profile identifiers
#define RTI_PROFILE_RTI                               0xFF
#define RTI_PROFILE_ID_START                          0x01
#define RTI_PROFILE_GDP                               0x00
#define RTI_PROFILE_ZRC                               0x01
#define RTI_PROFILE_ZID                               0x02
#define RTI_PROFILE_Z3S                               0x03
#define RTI_PROFILE_ID_END                            0x04

// CERC command codes
#define RTI_CERC_USER_CONTROL_PRESSED                 0x01
#define RTI_CERC_USER_CONTROL_REPEATED                0x02
#define RTI_CERC_USER_CONTROL_RELEASED                0x03
#define RTI_CERC_COMMAND_DISCOVERY_REQUEST            0x04
#define RTI_CERC_COMMAND_DISCOVERY_RESPONSE           0x05

// RC command codes
#define RTI_CERC_SELECT                               0x00
#define RTI_CERC_UP                                   0x01
#define RTI_CERC_DOWN                                 0x02
#define RTI_CERC_LEFT                                 0x03
#define RTI_CERC_RIGHT                                0x04
#define RTI_CERC_RIGHT_UP                             0x05
#define RTI_CERC_RIGHT_DOWN                           0x06
#define RTI_CERC_LEFT_UP                              0x07
#define RTI_CERC_LEFT_DOWN                            0x08
#define RTI_CERC_ROOT_MENU                            0x09
#define RTI_CERC_SETUP_MENU                           0x0A
#define RTI_CERC_CONTENTS_MENU                        0x0B
#define RTI_CERC_FAVORITE_MENU                        0x0C
#define RTI_CERC_EXIT                                 0x0D
  // note space
#define RTI_CERC_NUM_11                               0x1e
#define RTI_CERC_NUM_12                               0x1f
#define RTI_CERC_NUM_0                                0x20
#define RTI_CERC_NUM_1                                0x21
#define RTI_CERC_NUM_2                                0x22
#define RTI_CERC_NUM_3                                0x23
#define RTI_CERC_NUM_4                                0x24
#define RTI_CERC_NUM_5                                0x25
#define RTI_CERC_NUM_6                                0x26
#define RTI_CERC_NUM_7                                0x27
#define RTI_CERC_NUM_8                                0x28
#define RTI_CERC_NUM_9                                0x29
#define RTI_CERC_DOT                                  0x2A
#define RTI_CERC_ENTER                                0x2B
#define RTI_CERC_CLEAR                                0x2C
  // note space
#define RTI_CERC_NEXT_FAVORITE                        0x2F
#define RTI_CERC_CHANNEL_UP                           0x30
#define RTI_CERC_CHANNEL_DOWN                         0x31
#define RTI_CERC_PREVIOUS_CHANNEL                     0x32
#define RTI_CERC_SOUND_SELECT                         0x33
#define RTI_CERC_INPUT_SELECT                         0x34
#define RTI_CERC_DISPLAY_INFORMATION                  0x35
#define RTI_CERC_HELP                                 0x36
#define RTI_CERC_PAGE_UP                              0x37
#define RTI_CERC_PAGE_DOWN                            0x38
  // note space
#define RTI_CERC_POWER                                0x40
#define RTI_CERC_VOLUME_UP                            0x41
#define RTI_CERC_VOLUME_DOWN                          0x42
#define RTI_CERC_MUTE                                 0x43
#define RTI_CERC_PLAY                                 0x44
#define RTI_CERC_STOP                                 0x45
#define RTI_CERC_PAUSE                                0x46
#define RTI_CERC_RECORD                               0x47
#define RTI_CERC_REWIND                               0x48
#define RTI_CERC_FAST_FORWARD                         0x49
#define RTI_CERC_EJECT                                0x4A
#define RTI_CERC_FORWARD                              0x4B
#define RTI_CERC_BACKWARD                             0x4C
#define RTI_CERC_STOP_RECORD                          0x4D
#define RTI_CERC_PAUSE_RECORD                         0x4E
  // note space
#define RTI_CERC_ANGLE                                0x50
#define RTI_CERC_SUB_PICTURE                          0x51
#define RTI_CERC_VIDEO_ON_DEMAND                      0x52
#define RTI_CERC_ELECTRONIC_PROGRAM_GUIDE             0x53
#define RTI_CERC_TIMER_PROGRAMMING                    0x54
#define RTI_CERC_INITIAL_CONFIGURATION                0x55
  // note space
#define RTI_CERC_PLAY_FUNCTION                        0x60
#define RTI_CERC_PAUSE_PLAY_FUNCTION                  0x61
#define RTI_CERC_RECORD_FUNCTION                      0x62
#define RTI_CERC_PAUSE_RECORD_FUNCTION                0x63
#define RTI_CERC_STOP_FUNCTION                        0x64
#define RTI_CERC_MUTE_FUNCTION                        0x65
#define RTI_CERC_RESTORE_VOLUME_FUNCTION              0x66
#define RTI_CERC_TUNE_FUNCTION                        0x67
#define RTI_CERC_SELECT_MEDIA_FUNCTION                0x68
#define RTI_CERC_SELECT_AV_INPUT_FUNCTION             0x69
#define RTI_CERC_SELECT_AUDIO_INPUT_FUNCTION          0x6A
#define RTI_CERC_POWER_TOGGLE_FUNCTION                0x6B
#define RTI_CERC_POWER_OFF_FUNCTION                   0x6C
#define RTI_CERC_POWER_ON_FUNCTION                    0x6D
  // note space
#define RTI_CERC_F1_BLUE                              0x71
#define RTI_CERC_F2_RED                               0x72
#define RTI_CERC_F3_GREEN                             0x73
#define RTI_CERC_F4_YELLOW                            0x74
#define RTI_CERC_F5                                   0x75
#define RTI_CERC_DATA                                 0x76
  // one reserved code for special use
#define RTI_CERC_RESERVED_1                           0xFF

  // play function can be extended to two byte command
#define RTI_CERC_EXTENDED_COMMAND(_cmd) \
  ((_cmd) == RTI_CERC_PLAY_FUNCTION)

  // the following functions add selection as a single byte following
  // the command
#define RTI_CERC_SELECTION_COMMAND(_cmd) \
  (((_cmd) == RTI_CERC_SELECT_MEDIA_FUNCTION) || \
   ((_cmd) == RTI_CERC_SELECT_AV_INPUT_FUNCTION) || \
   ((_cmd) == RTI_CERC_SELECT_AUDIO_INPUT_FUNCTION))

  // the following command has composite payload, which shall not be supported
  // in a basic RF remote controller
#define RTI_CERC_COMPOSITE_COMMAND(_cmd) \
  ((_cmd) == RTI_CERC_TUNE_FUNCTION)

  // play function payload definition
#define RTI_CERC_PLAY_MODE_PLAY_FORWARD               0x24
#define RTI_CERC_PLAY_MODE_PLAY_REVERSE               0x20
#define RTI_CERC_PLAY_MODE_PLAY_STILL                 0x25
#define RTI_CERC_PLAY_MODE_FAST_FORWARD_MINIMUM_SPEED 0x05
#define RTI_CERC_PLAY_MODE_FAST_FORWARD_MEDIUM_SPEED  0x06
#define RTI_CERC_PLAY_MODE_FAST_FORWARD_MAXIMUM_SPEED 0x07
#define RTI_CERC_PLAY_MODE_FAST_REVERSE_MINIMUM_SPEED 0x09
#define RTI_CERC_PLAY_MODE_FAST_REVERSE_MEDIUM_SPEED  0x0A
#define RTI_CERC_PLAY_MODE_FAST_REVERSE_MAXIMUM_SPEED 0x0B
#define RTI_CERC_PLAY_MODE_SLOW_FORWARD_MINIMUM_SPEED 0x15
#define RTI_CERC_PLAY_MODE_SLOW_FORWARD_MEDIUM_SPEED  0x16
#define RTI_CERC_PLAY_MODE_SLOW_FORWARD_MAXIMUM_SPEED 0x17
#define RTI_CERC_PLAY_MODE_SLOW_REVERSE_MINIMUM_SPEED 0x19
#define RTI_CERC_PLAY_MODE_SLOW_REVERSE_MEDIUM_SPEED  0x1A
#define RTI_CERC_PLAY_MODE_SLOW_REVERSE_MAXIMUM_SPEED 0x1B


  // TI vendor specific command protocol identifiers
#define RTI_PROTOCOL_POLL                             0x00
#define RTI_PROTOCOL_OAD                              0x10 // over the air download
#define RTI_PROTOCOL_TEST                             0x20
#define RTI_PROTOCOL_EXT_TV_RC                        0x30 // extended TV remote control
#define RTI_PROTOCOL_PLAY_LIST_DOWNLOAD               0x40

  // TI test protocol command identifiers (use with RTI_PROTOCOL_TEST)
#define RTI_CMD_TEST_PARAMETERS                       0x00
#define RTI_CMD_TEST_DATA                             0x01
#define RTI_CMD_TEST_REPORT                           0x02
#define RTI_CMD_TEST_DATA_SEQUENCED                   0x03

  // TI extended TV remote control command identifiers (use with RTI_PROTOCOL_EXT_TV_RC)
#define RTI_CMD_EXT_TV_ZOOM_IN                        0x00
#define RTI_CMD_EXT_TV_ZOOM_OUT                       0x01
#define RTI_CMD_EXT_TV_PIP_TOGGLE                     0x02
#define RTI_CMD_EXT_TV_PIP_SWAP                       0x03
#define RTI_CMD_EXT_TV_PIP_MOVE                       0x04
#define RTI_CMD_EXT_TV_PIP_SIZE                       0x05
#define RTI_CMD_EXT_TV_PIP_CH_DOWN                    0x06
#define RTI_CMD_EXT_TV_PIP_CH_UP                      0x07
#define RTI_CMD_EXT_TV_PIP_FREEZE                     0x08

  // TI specific profile
  // Use of such profile identifier is not clear at the moment.
  // TI vendor specific commands will hence be profile neutral.
#define RTI_PROFILE_TI                                0xC0

#ifdef __cplusplus
}
#endif

#endif // RTI_CONSTANTS_H
