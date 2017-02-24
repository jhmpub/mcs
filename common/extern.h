
// initializing to the maximum volume levels insures sufficient "down"
// volume commands will be issued during ampInit to reach volume floors
extern int masterVolume;
extern int centerVolume;
extern int subwooferVolume;
extern int surroundRightVolume;
extern int surroundLeftVolume;
extern int debug;
extern int initializing;
extern int audioSource;
extern int station;
extern int currentEffectSpeaker;
extern int audioPowerState, muteState;
extern int registrationCount, surroundProfile;
extern int channel_a_director, rear_speaker_selector, channel_a_relay;
extern int bSpeakerState, subwoofer_relay, center_speaker_relay;
extern int rearSpeakerState, frontSpeakerState;
extern int subwooferState, subwooferRelayState, LFE_State;
extern int hdmiSwitchState;
extern int ttyExit;
extern int undefined;
extern HMODULE handle;
extern INPUT_RECORD inputQ[3];
extern volatile LONG lock;

extern tira_init_t              tira_init;
extern tira_cleanup_t           tira_cleanup;
extern tira_set_handler_t       tira_set_handler;
extern tira_start_t             tira_start;
extern tira_stop_t              tira_stop;
extern tira_start_capture_t     tira_start_capture;
extern tira_cancel_capture_t    tira_cancel_capture;
extern tira_get_captured_data_t tira_get_captured_data;
extern tira_transmit_t          tira_transmit;
extern tira_delete_t            tira_delete;

extern struct serverState {
    int rearSpeakerState; 
    int frontSpeakerState; 
    int bSpeakerState;
    int audioSource;
} serverState, *preserved;        

extern struct irCmd {
    int id;
    const char * sz;
    int * state;
    int size;   // number of bytes in the codes array
    const unsigned char * codes;
} irCmd[];



