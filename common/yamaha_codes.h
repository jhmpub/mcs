#ifndef YAMAHA_CODES_H
#define YAMAHA_CODES_H

#define AUDIO_POWER_ON_OFF_DELAY 100  // pause 100 milliseconds before and after the power command
#define DSP_CMD_DELAY 47              // pause before and after DSP mode commands

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

#endif  // ifndef YAMAHA_CODES_H

