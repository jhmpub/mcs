#ifdef __GNUC__
#include <stdarg.h>
#include <winbase.h>
#endif

int __stdcall irCmdCallback(const char *);
int keyToTiraCmdId(const char);
static char captureActive, callbackActive;
struct irMsg cmd2irMsg(int);
void menu(void);
void sendIrCmd(char);
void sendIrCmd(struct tiraCmd *);

struct tiraCmdMap {
   const char key;
   int id;
   const char * description;
}  tiraCmdMap[] = {
   {'a',              UNDEFINED,                       "activates capture mode"},
   {'b',              UNDEFINED,                       "activate the capture callback method"},
   {'c',              UNDEFINED,                       "cancels capture mode"},
   {'!',              UNDEFINED,                       "transmit the dynamic IR code"},
   {'i',              AUDIO_POWER_ON,                  SZ_AUDIO_POWER_ON},
   {'o',              AUDIO_POWER_OFF,                 SZ_AUDIO_POWER_OFF},
   {'f',              FM,                              SZ_FM},
   {'g',              GEORGIA,                         SZ_GEORGIA},
   {'j',              JIMSON,                          SZ_JIMSON},
   {'t',              TV,                              SZ_TV},
   {'v',              DVD,                             SZ_DVD},
   {'y',              VIDEO_AUX,                       SZ_VIDEO_AUX},
   {'u',              MASTER_VOLUME_UP,                SZ_MASTER_VOLUME_UP},
   {'d',              MASTER_VOLUME_DOWN,              SZ_MASTER_VOLUME_DOWN},
   {'m',              MUTE_TOGGLE,                     SZ_MUTE_TOGGLE},
   {')',              CHANNEL_A_TO_FRONT_SPEAKERS,     SZ_CHANNEL_A_TO_FRONT_SPEAKERS}, 
   {'(',              CHANNEL_A_TO_REAR_SPEAKERS,      SZ_CHANNEL_A_TO_REAR_SPEAKERS}, 
   {']',              REAR_SPEAKERS_FROM_CHANNEL_A,    SZ_REAR_SPEAKERS_FROM_CHANNEL_A}, 
   {'[',              REAR_SPEAKERS_FROM_REAR_CHANNEL, SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL},
   {'+',              CHANNEL_A_ON,                    SZ_CHANNEL_A_ON}, 
   {'-',              CHANNEL_A_OFF,                   SZ_CHANNEL_A_OFF}, 
   {'>',              CHANNEL_B_ON,                    SZ_CHANNEL_B_ON}, 
   {'<',              CHANNEL_B_OFF,                   SZ_CHANNEL_B_OFF}, 
   {'}',              SUBWOOFER_ON,                    SZ_SUBWOOFER_ON}, 
   {'{',              SUBWOOFER_OFF,                   SZ_SUBWOOFER_OFF}, 
   {';',              DSP_EFFECT_TOGGLE,               SZ_DSP_EFFECT_TOGGLE},
   {':',              DSP_DOLBY_NORMAL,                SZ_DSP_DOLBY_NORMAL},
   {'9',              HDMI_SET_TOP_BOX_SOURCE,         SZ_HDMI_SET_TOP_BOX_SOURCE},
   {'0',              HDMI_DVD_SOURCE,                 SZ_HDMI_DVD_SOURCE},
   {'_',              HDMI_DVR_SOURCE,                 SZ_HDMI_DVR_SOURCE},
   {'1',              FM1,                             SZ_FM1},
   {'2',              FM2,                             SZ_FM2},
   {'3',              FM3,                             SZ_FM3},
   {'4',              FM4,                             SZ_FM4},
   {'5',              FM5,                             SZ_FM5},
   {'6',              FM6,                             SZ_FM6},
   {'7',              FM7,                             SZ_FM7},
   {'8',              FM8,                             SZ_FM8},
   {'l',              MENU_EFFECT_SPEAKER_LEVEL,       SZ_MENU_EFFECT_SPEAKER_LEVEL},
   {'s',              MENU_SETTINGS,                   SZ_MENU_SETTINGS},
   {CHAR_ARROW_UP,    MENU_UP,                         SZ_MENU_UP},
   {CHAR_ARROW_DOWN,  MENU_DOWN,                       SZ_MENU_DOWN},
   {CHAR_ARROW_RIGHT, MENU_RIGHT,                      SZ_MENU_RIGHT},
   {CHAR_ARROW_LEFT,  MENU_LEFT,                       SZ_MENU_LEFT},
   {'q',              UNDEFINED,                       "quit"},
}; 

                      
                      