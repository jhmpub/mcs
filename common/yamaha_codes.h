#ifndef YAMAHA_CODES_H
#define YAMAHA_CODES_H

#define POWER_ON_OFF_DELAY 100  // pause 100 milliseconds before and after the power command
#define DSP_CMD_DELAY 50        // pause 50 milliseconds before and after DSP mode commands

#define POWER_ON 1
#define POWER_OFF 2
#define GEORGIA 3   // input: CBL/SAT
#define JIMSON 4    // input: CD 
#define LVR_MM 5    // input: CD-R
#define DEN_MM 6    // input: V-AUX
#define FM  7       // input: TUNER
#define FM1 8       // preset station A1
#define FM2 9       // preset station A2
#define FM3 10      // preset station A3
#define FM4 11      // preset station A4
#define FM5 12      // preset station A5
#define FM6 13      // preset station A6
#define FM7 14      // preset station A7
#define FM8 15      // preset station A8 
#define MUTE_ON 16
#define MUTE_OFF 17
#define MUTE_TOGGLE 18
#define FIRST_REGISTRATION 19
#define REGISTER 20
#define DEREGISTER 21
#define TV 22       // amplifier input: D-TV/LD
#define DVD 23      // amplifier input: DVD
#define VIDEO_AUX 24
#define SURROUND_MIN        25
#define SURROUND_STD        26
#define SURROUND_MAX        27

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


// 500-599 reserved for yamaha digital sound processor modes
// The DSP_ON and DSP_OFF use the same DSP_TOGGLE command
// DSP_ON provides 2 channel upmixing to 5 channel sound
// in the "Enhanced" sub-mode.  Use the yamaha remote and press
// "Dolby/DTS SUR" to toggle the display between "Normal" and "Enhanced".
// Sub-mode settings are "sticky" which means they are retained 
// when switching between modes and the standby state.  Volatile storage
// is retained for a week if power is lost.
#define DSP_MODE_CMD_RANGE_BEGIN  500
#define DSP_DOLBY_NORMAL          500   // Dolby/DTS SUR Normal
#define DSP_EFFECT_ON             501   // Effect On enables the current DSP program
#define DSP_EFFECT_OFF            502   // Effect Off disables the current DSP program
#define DSP_EFFECT_TOGGLE         503   // the EFFECT IR cmd is actually a toggle 
#define DSP_EFFECT_TOGGLE_ENABLE  504   // allows toggling input source DSP on and off
#define DSP_EFFECT_TOGGLE_DISABLE 505   // disallows toggling input source DSP on and off
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
                               
#define SZ_DEFAULT_MASTER_VOLUME "47"
#define SZ_DEFAULT_CENTER_VOLUME "12"
#define SZ_DEFAULT_SURROUND_VOLUME "20"
#define SZ_DEFAULT_SUBWOOFER_VOLUME "12"

#define SZ_POWER_ON "power on"
#define SZ_POWER_OFF "power off"
#define SZ_GEORGIA "georgia"
#define SZ_JIMSON "jimson"
#define SZ_LVR_MM "lvr-mm"
#define SZ_DEN_MM "den-mm"
#define SZ_FM  "fm"
#define SZ_FM1 "fm1"
#define SZ_FM2 "fm2"
#define SZ_FM3 "fm3"
#define SZ_FM4 "fm4"
#define SZ_FM5 "fm5"
#define SZ_FM6 "fm6"
#define SZ_FM7 "fm7"
#define SZ_FM8 "fm8"
#define SZ_MUTE_ON "mute on"
#define SZ_MUTE_OFF "mute off"
#define SZ_MUTE_TOGGLE "mute toggle"
#define SZ_REGISTER "register"
#define SZ_DEREGISTER "deregister"
#define SZ_TV  "tv"
#define SZ_DVD  "dvd"
#define SZ_VIDEO_AUX "video aux"
#define SZ_FIRST_REGISTRATION "first registration"
#define SZ_ENABLE_CONTROLS "enable controls"
#define SZ_SURROUND_MIN  "surround min"
#define SZ_SURROUND_STD  "surround std"
#define SZ_SURROUND_MAX  "surround max"
#define SZ_MENU_EFFECT_SPEAKER_LEVEL "effect speaker level menu"
#define SZ_MENU_SETTINGS  "settings menu"
#define SZ_MENU_UP    "menu up"
#define SZ_MENU_DOWN  "menu down"
#define SZ_MENU_LEFT  "menu left"
#define SZ_MENU_RIGHT "menu right"
#define SZ_VOLUME                      " volume "
#define SZ_VOLUME_UP                   SZ_VOLUME "up"                 
#define SZ_VOLUME_DOWN                 SZ_VOLUME "down"               
#define SZ_MASTER_VOLUME               "master" SZ_VOLUME             
#define SZ_MASTER_VOLUME_UP            "master" SZ_VOLUME_UP          
#define SZ_MASTER_VOLUME_DOWN          "master" SZ_VOLUME_DOWN        
#define SZ_CENTER_VOLUME               "center" SZ_VOLUME             
#define SZ_CENTER_VOLUME_UP            "center" SZ_VOLUME_UP          
#define SZ_CENTER_VOLUME_DOWN          "center" SZ_VOLUME_DOWN        
#define SZ_SURROUND_VOLUME             "surround" SZ_VOLUME     
#define SZ_SURROUND_RIGHT_VOLUME       "surround right" SZ_VOLUME     
#define SZ_SURROUND_RIGHT_VOLUME_UP    "surround right" SZ_VOLUME_UP  
#define SZ_SURROUND_RIGHT_VOLUME_DOWN  "surround right" SZ_VOLUME_DOWN
#define SZ_SURROUND_LEFT_VOLUME        "surround left" SZ_VOLUME      
#define SZ_SURROUND_LEFT_VOLUME_UP     "surround left" SZ_VOLUME_UP   
#define SZ_SURROUND_LEFT_VOLUME_DOWN   "surround left" SZ_VOLUME_DOWN 
#define SZ_SUBWOOFER_VOLUME            "subwoofer" SZ_VOLUME          
#define SZ_SUBWOOFER_VOLUME_UP         "subwoofer" SZ_VOLUME_UP       
#define SZ_SUBWOOFER_VOLUME_DOWN       "subwoofer" SZ_VOLUME_DOWN     
#define SZ_DSP_EFFECT_ON  "dsp effect on"
#define SZ_DSP_EFFECT_OFF "dsp effect off"
#define SZ_DSP_EFFECT_TOGGLE "dsp effect toggle"
#define SZ_DSP_DOLBY_NORMAL "dsp dolby normal"

#endif  // ifndef YAMAHA_CODES_H

