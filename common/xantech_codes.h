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

#ifndef XANTECH_CODES_H
#define XANTECH_CODES_H 

#define XANTECH_DELAY 50   // pause before and after each xantech command to insure distinction
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

#endif  // ifndef XANTECH_CODES_H
