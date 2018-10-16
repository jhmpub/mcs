#ifndef TIRA_TX_H
#define TIRA_TX_H

//// begin xantech device definitions

// The IR command code char arrays were collected using the demo utility
// capture function.
// The OFF, ON, TGL and MMT index values map to an array of IR codes
// captured for a sub-group identifier configured on each xantech device.
// Sub-group identifiers are zero based so:
// ab switch #1 -> sub group 0
// ab switch #2 -> sub group 1
// on/off switch #3 -> sub group 2
// etc...

// 0-99 reserved for amplifier codes

#define XANTECH_DELAY 75   // pause before and after each xantech command to insure distinction
#define MATRIX_CMD_RANGE_BEGIN  100
#define XANTECH_CMD_RANGE_BEGIN 100

// ab switch #1 channel a director wired with one input, two outputs
#define CHANNEL_A_TO_FRONT_SPEAKERS 100    // A
#define CHANNEL_A_TO_REAR_SPEAKERS  101    // B
#define OFF0 100
#define ON0  101
#define TGL0 102
#define MMT0 103

// ab switch #2 rear speaker selector wired with two inputs, one output
#define REAR_SPEAKERS_FROM_CHANNEL_A    104  // A
#define REAR_SPEAKERS_FROM_REAR_CHANNEL 105  // B
#define OFF1 104
#define ON1  105
#define TGL1 106
#define MMT1 107

// on/off switch #3 channel a relay enables or disables primary main speaker pair
#define CHANNEL_A_OFF 108  // wired to N/O so OFF by default
#define CHANNEL_A_ON  109
#define OFF2 108
#define ON2  109
#define TGL2 110
#define MMT2 111

// on/off switch #4 center speaker relay enables or disables the center speaker
#define CENTER_SPEAKER_OFF 112  // wired to N/O so OFF by default
#define CENTER_SPEAKER_ON  113
#define OFF3 112
#define ON3  113
#define TGL3 114
#define MMT3 115

#define MATRIX_CMD_RANGE_END 199

// on/off switch #5 channel b relay enables or disables secondary main speaker pair
#define CHANNEL_B_OFF 212  // wired to N/O so OFF by default
#define CHANNEL_B_ON  213
#define OFF4 212
#define ON4  213
#define TGL4 214
#define MMT4 215

// on/off switch #6 subwoofer relay enables or disables the subwoofer speaker
#define SUBWOOFER_OFF 216
#define SUBWOOFER_ON  217
#define OFF5 216
#define ON5  217
#define TGL5 218
#define MMT5 219

#define OFF6 224
#define ON6  225
#define TGL6 226
#define MMT6 227

#define OFF7 228
#define ON7  229
#define TGL7 230
#define MMT7 231

#define XANTECH_CMD_RANGE_END 299

#define B_SPEAKERS_ON      300
#define B_SPEAKERS_OFF     301
#define REAR_SPEAKERS_ON   302
#define REAR_SPEAKERS_OFF  303
#define FRONT_SPEAKERS_ON  304
#define FRONT_SPEAKERS_OFF 305
#define SZ_B_SPEAKERS_ON "b speakers on"
#define SZ_B_SPEAKERS_OFF "b speakers off"
#define SZ_REAR_SPEAKERS_ON "rear speakers on"
#define SZ_REAR_SPEAKERS_OFF "rear speakers off"
#define SZ_FRONT_SPEAKERS_ON "front speakers on"
#define SZ_FRONT_SPEAKERS_OFF "front speakers off"


// 400- reserved for amplifier codes

#define SZ_CHANNEL_A_TO_FRONT_SPEAKERS "channel a to front speakers"
#define SZ_CHANNEL_A_TO_REAR_SPEAKERS "channel a to rear speakers"
#define SZ_OFF0 "Relay 0 Off"
#define SZ_ON0 "Relay 0 On"
#define SZ_TGL0 "Relay 0 Toggle"
#define SZ_MMT0 "Relay 0 Momentary"

#define SZ_REAR_SPEAKERS_FROM_CHANNEL_A "rear speakers from channel a"
#define SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL "rear speakers from rear channel"
#define SZ_OFF1 "Relay 1 Off"
#define SZ_ON1 "Relay 1 On"
#define SZ_TGL1 "Relay 1 Toggle"
#define SZ_MMT1 "Relay 1 Momentary"

#define SZ_CHANNEL_A_ON "channel a on"
#define SZ_CHANNEL_A_OFF "channel a off"
#define SZ_OFF2 "Relay 2 Off"
#define SZ_ON2 "Relay 2 On"
#define SZ_TGL2 "Relay 2 Toggle"
#define SZ_MMT2 "Relay 2 Momentary"

#define SZ_CHANNEL_B_ON "channel b on"
#define SZ_CHANNEL_B_OFF "channel b off"
#define SZ_OFF3 "Relay 3 Off"
#define SZ_ON3 "Relay 3 On"
#define SZ_TGL3 "Relay 3 Toggle"
#define SZ_MMT3 "Relay 3 Momentary"

#define SZ_SUBWOOFER_ON "subwoofer on"
#define SZ_SUBWOOFER_OFF "subwoofer off"
#define SZ_OFF4 "Relay 4 Off"
#define SZ_ON4 "Relay 4 On"
#define SZ_TGL4 "Relay 4 Toggle"
#define SZ_MMT4 "Relay 4 Momentary"

#define SZ_CENTER_SPEAKER_ON  "center speaker on"
#define SZ_CENTER_SPEAKER_OFF "center speaker off"
#define SZ_OFF5 "Relay 5 Off"
#define SZ_ON5 "Relay 5 On"
#define SZ_TGL5 "Relay 5 Toggle"
#define SZ_MMT5 "Relay 5 Momentary"

#define SZ_OFF6 "Relay 6 Off"
#define SZ_ON6 "Relay 6 On"
#define SZ_TGL6 "Relay 6 Toggle"
#define SZ_MMT6 "Relay 6 Momentary"

#define SZ_OFF7 "Relay 7 Off"
#define SZ_ON7 "Relay 7 On"
#define SZ_TGL7 "Relay 7 Toggle"
#define SZ_MMT7 "Relay 7 Momentary"

#define OFF 0
#define ON 1
#define MMT 2
#define TGL 3

#define GRP0 0 
#define GRP1 1 
#define GRP2 2 
#define GRP3 3 
#define GRP4 4 
#define GRP5 5 
#define GRP6 6 
#define GRP7 7 


//// end xantech device definitions


/// begin yamaha receiver definitions

#define AUDIO_POWER_ON_OFF_DELAY 250  // delay sending ir commands to allow power on init
#define DSP_CMD_DELAY 50              // insure distinction between DSP mode menu commands

// 100-399 reserved for xantech codes

// 400-500 reserved for yamaha menu based features
#define MENU_RANGE_BEGIN              400
#define MENU_EFFECT_SPEAKER_LEVEL     400
#define MENU_SETTINGS                 401
#define MENU_UP                       402
#define MENU_DOWN                     403
#define MENU_LEFT                     404
#define MENU_RIGHT                    405
#define MENU_RANGE_END                499


// 500-599 is reserved for yamaha digital sound processor modes.
//
// The DSP_EFFECT_ON and DSP_EFFECT_OFF commands both use the same 
// DSP_EFFECT_TOGGLE command.
//
// Pressing "Effect" toggles between "Effect Off" and the DSP setting for 
// the current source.
//
// Pressing "Dolby/DTS SUR" once enables the most recent "Normal" or "Enhanced" 
// DSP setting.  Pressing "Dolby/DTS SUR" twice within three seconds toggles 
// between the "Normal" and "Enhanced" sub-modes.  
//
// DSP settings are "sticky" which means they are retained when switching 
// between modes and the standby state.  Volatile storage is retained for a  
// week if power is lost.
#define DSP_MODE_CMD_RANGE_BEGIN  500
#define DSP_DOLBY_NORMAL          500   // Dolby/DTS SUR Normal
#define DSP_EFFECT_ON             501   // decode 5 channel and upmix 2 channel sources
#define DSP_EFFECT_OFF            502   // decode 2 channel and downmix 5 channel sources 
#define DSP_EFFECT_TOGGLE         503   // toggle between "Effect Off" and "Normal/Enhanced" 
#define DSP_EFFECT_TOGGLE_ENABLE  504   // enable DSP_EFFECT_TOGGLE on a source 
#define DSP_EFFECT_TOGGLE_DISABLE 505   // disable DSP_EFFECT_TOGGLE on a source
#define DSP_MODE_CMD_RANGE_END    599


// 600-699 reserved for volume level commands
#define VOLUME_RANGE_BEGIN            600
#define MASTER_VOLUME                 600
#define MASTER_VOLUME_UP              601
#define MASTER_VOLUME_DOWN            602
#define CENTER_VOLUME                 603
#define CENTER_VOLUME_UP              604
#define CENTER_VOLUME_DOWN            605
#define SURROUND_VOLUME               606
#define SURROUND_RIGHT_VOLUME         609
#define SURROUND_RIGHT_VOLUME_UP      610
#define SURROUND_RIGHT_VOLUME_DOWN    611
#define SURROUND_LEFT_VOLUME          612
#define SURROUND_LEFT_VOLUME_UP       613
#define SURROUND_LEFT_VOLUME_DOWN     614
#define SUBWOOFER_VOLUME              615
#define SUBWOOFER_VOLUME_UP           616
#define SUBWOOFER_VOLUME_DOWN         617
#define VOLUME_RANGE_END              699

#define MASTER_VOLUME_MAX 100
#define EFFECT_VOLUME_MAX 20

#define MASTER_VOLUME_THRESHOLD_SURROUND_STD 40
#define MASTER_VOLUME_THRESHOLD_SURROUND_MAX 55
                               
#define SZ_DEFAULT_MASTER_VOLUME "40"
#define SZ_DEFAULT_CENTER_VOLUME "15"
#define SZ_DEFAULT_CENTER_VOLUME_DVD "20"
#define SZ_DEFAULT_SURROUND_VOLUME "20"
#define SZ_DEFAULT_SUBWOOFER_VOLUME "12"

#define SZ_MENU_EFFECT_SPEAKER_LEVEL "effect speaker level menu"
#define SZ_MENU_SETTINGS  "settings menu"
#define SZ_MENU_UP    "menu up"
#define SZ_MENU_DOWN  "menu down"
#define SZ_MENU_LEFT  "menu left"
#define SZ_MENU_RIGHT "menu right"
#define SZ_VOLUME                      "volume "
#define SZ_VOLUME_UP                   SZ_VOLUME "up"                 
#define SZ_VOLUME_DOWN                 SZ_VOLUME "down"               
#define SZ_MASTER_VOLUME               "master " SZ_VOLUME             
#define SZ_MASTER_VOLUME_UP            "master " SZ_VOLUME_UP          
#define SZ_MASTER_VOLUME_DOWN          "master " SZ_VOLUME_DOWN        
#define SZ_CENTER_VOLUME               "center " SZ_VOLUME             
#define SZ_CENTER_VOLUME_UP            "center " SZ_VOLUME_UP          
#define SZ_CENTER_VOLUME_DOWN          "center " SZ_VOLUME_DOWN        
#define SZ_SURROUND_VOLUME             "surround " SZ_VOLUME     
#define SZ_SURROUND_RIGHT_VOLUME       "surround right " SZ_VOLUME     
#define SZ_SURROUND_RIGHT_VOLUME_UP    "surround right " SZ_VOLUME_UP  
#define SZ_SURROUND_RIGHT_VOLUME_DOWN  "surround right " SZ_VOLUME_DOWN
#define SZ_SURROUND_LEFT_VOLUME        "surround left " SZ_VOLUME      
#define SZ_SURROUND_LEFT_VOLUME_UP     "surround left " SZ_VOLUME_UP   
#define SZ_SURROUND_LEFT_VOLUME_DOWN   "surround left " SZ_VOLUME_DOWN 
#define SZ_SUBWOOFER_VOLUME            "subwoofer " SZ_VOLUME          
#define SZ_SUBWOOFER_VOLUME_UP         "subwoofer " SZ_VOLUME_UP       
#define SZ_SUBWOOFER_VOLUME_DOWN       "subwoofer " SZ_VOLUME_DOWN     
#define SZ_DSP_EFFECT_ON  "dsp effect on"
#define SZ_DSP_EFFECT_OFF "dsp effect off"
#define SZ_DSP_EFFECT_TOGGLE "dsp effect toggle"
#define SZ_DSP_EFFECT_TOGGLE_ENABLE  "dsp effect toggle enable"
#define SZ_DSP_EFFECT_TOGGLE_DISABLE "dsp effect toggle disable"
#define SZ_DSP_DOLBY_NORMAL "dsp dolby normal"

//// end yamaha receiver definitions

#endif // ifndef TIRA_TX_H
