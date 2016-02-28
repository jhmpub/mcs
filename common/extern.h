
// initializing to the maximum volume levels insures sufficient "down"
// volume commands will be issued during ampInit to reach volume floors
extern const unsigned char * Data;
extern int masterVolume;
extern int centerVolume;
extern int subwooferVolume;
extern int surroundRightVolume;
extern int surroundLeftVolume;
extern int initializing;
extern int inputSource;
extern int station;
extern int currentEffectSpeaker;
extern int powerState, muteState, poweringOff;
extern int registrationCount, surroundProfile;
extern int channelA_Relay, channelA_Switch, rearSpeakerSwitch;
extern int rearSpeakerState, frontSpeakerState, denSpeakerState;
extern int subwooferState, LFE_State;
extern int undefined;
extern HMODULE handle;
extern SOCKET txSocket;

extern struct serverState {
    int rearSpeakerState; 
    int frontSpeakerState; 
    int denSpeakerState;
    int inputSource;
} serverState, *preserved;        

extern struct dsp { // digital sound processor
    int inputSource;
    const int program;
    int state;            // DSP_EFFECT_ON or DSP_EFFECT_OFF
    const int toggle;     // whether state can change after init
} dsp[];

extern struct irCmd {
    int id;
    char * sz;
    int * state;
    int size;   // number of bytes in the codes array
    unsigned char const * codes;
} irCmd[];

