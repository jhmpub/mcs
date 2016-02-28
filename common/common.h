
#ifndef COMMON_H
#define COMMON_H

#define AUDIO_CONTROL_REQUEST_PORT (short) 2000
#define AUDIO_CONTROL_NOTIFICATION_PORT (short) 2001
#define AUDIO_EFFECT_CONTROL_NOTIFICATION_PORT (short) 2002
#define AUDIO_REMOTE_CONTROL_NOTIFICATION_PORT (short) 2003
#define AUDIO_CONTROL_SERVER_HOSTNAME "jimson"
#define AUDIO_CONTROL_UNIFIED_HOSTNAME "localhost"

#define DEFAULT_COM_PORT 5

#define UNINITIALIZED 0
#define UNDEFINED -1
#define EXIT -2

#define SZ_UNINITIALIZED "uninitialized"
#define SZ_UNDEFINED "undefined"
#define SZ_EXIT "exit"

extern char tira_dll_absolute_path[];

BOOL isValidCmdId(int);
BOOL isCmdStateSet(int);
char * cmdIdToSz(int);
int cmdSzToId(char *);
int getDspState(void);
int irTransmit(int);
struct dsp * getDsp();
struct irCmd * cmdIdToIrCmd(int);
void initDefaultInputSourceDspProgram(void);
void datagramSend(char *, LPSOCKADDR);
void setState(int);
void delay(int);
void loadTiraDll(void);
void printErrorAndExit(char *);
void printWinsockError(char *);
void unloadTiraDll(void);


#endif  // ifndef COMMON_H
