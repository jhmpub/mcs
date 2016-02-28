// The IR command code char arrays were collected using the demo utility
// capture function.
// The index appending the OFF, ON, TGL and MMT commands references 
// the sub-group identifier for devices on the same IR bus.

// 0-99 reserved for amplifier codes

#ifndef XANTECH_CODES_H
#define XANTECH_CODES_H 

#define XANTECH_DELAY 75   // pause before and after each xantech command to insure distinction
#define MATRIX_CMD_RANGE_BEGIN 100
#define XANTECH_CMD_RANGE_BEGIN 100

// ab switch #1 wired with one input, two outputs
#define CHANNEL_A_TO_FRONT_SPEAKERS 100    // A
#define CHANNEL_A_TO_AB_SWITCH   101    // B
#define OFF0 100
#define ON0  101
#define TGL0 102
#define MMT0 103


// ab switch #2 wired with two inputs, one output
#define REAR_SPEAKERS_FROM_AB_SWITCH  104  // A
#define REAR_SPEAKERS_FROM_REAR_CHANNEL 105  // B
#define OFF1 104
#define ON1  105
#define TGL1 106
#define MMT1 107


// relay switch #3 enables or disables channel a rear or living rm speakers
#define CHANNEL_A_RELAY_OFF 108  // wired to N/O so OFF by default
#define CHANNEL_A_RELAY_ON  109
#define OFF2 108
#define ON2  109
#define TGL2 110
#define MMT2 111

#define MATRIX_CMD_RANGE_END 199

// relay switch #4 enables or disables channel b speakers
#define CHANNEL_B_SPEAKERS_OFF 212  // wired to N/O so OFF by default
#define CHANNEL_B_SPEAKERS_ON  213
#define OFF3 212
#define ON3  213
#define TGL3 214
#define MMT3 215

#define SUBWOOFER_OFF 216
#define SUBWOOFER_ON  217
#define OFF4 216
#define ON4  217
#define TGL4 218
#define MMT4 219

#define OFF5 220
#define ON5  221
#define TGL5 222
#define MMT5 223

#define OFF6 224
#define ON6  225
#define TGL6 226
#define MMT6 227

#define OFF7 228
#define ON7  229
#define TGL7 230
#define MMT7 231

#define XANTECH_CMD_RANGE_END 299

#define REAR_SPEAKERS_ON   300
#define REAR_SPEAKERS_OFF  301
#define FRONT_SPEAKERS_ON  302
#define FRONT_SPEAKERS_OFF 303
#define SZ_REAR_SPEAKERS_ON "rear speakers on"
#define SZ_REAR_SPEAKERS_OFF "rear speakers off"
#define SZ_FRONT_SPEAKERS_ON "front speakers on"
#define SZ_FRONT_SPEAKERS_OFF "front speakers off"

// 400- reserved for amplifier codes


#define SZ_CHANNEL_A_TO_FRONT_SPEAKERS "channel a to front speakers"
#define SZ_CHANNEL_A_TO_AB_SWITCH "channel a to ab switch"
#define SZ_OFF0 "Relay 0 Off"
#define SZ_ON0 "Relay 0 On"
#define SZ_TGL0 "Relay 0 Toggle"
#define SZ_MMT0 "Relay 0 Momentary"


#define SZ_REAR_SPEAKERS_FROM_AB_SWITCH "rear speakers from ab switch"
#define SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL "rear speakers from rear channel"
#define SZ_OFF1 "Relay 1 Off"
#define SZ_ON1 "Relay 1 On"
#define SZ_TGL1 "Relay 1 Toggle"
#define SZ_MMT1 "Relay 1 Momentary"


#define SZ_CHANNEL_A_RELAY_ON "channel a relay on"
#define SZ_CHANNEL_A_RELAY_OFF "channel a relay off"
#define SZ_OFF2 "Relay 2 Off"
#define SZ_ON2 "Relay 2 On"
#define SZ_TGL2 "Relay 2 Toggle"
#define SZ_MMT2 "Relay 2 Momentary"


#define SZ_CHANNEL_B_SPEAKERS_ON "channel b speakers on"
#define SZ_CHANNEL_B_SPEAKERS_OFF "channel b speakers off"
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
