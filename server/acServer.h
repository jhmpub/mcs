
BOOL consoleModeExitHandler( DWORD fdwCtrlType );
BOOL isCmdStateSet(int);
BOOL isConsoleMode(void);
BOOL isValidCmdId(int);

char * cmdIdToSz(int);
char * getClientMsg(void);

HANDLE initWindowsModeExitHandler(void);

int cmdSzToId(char *);
int getDspMode(void);
int getDspProgram(void);
int getDspState(void);
int getDspToggle(void);
int processArgs(int, char **);

LRESULT WINAPI windowsModeExitHandler(HWND, UINT, WPARAM, LPARAM);

char * usage = "\nUsage: acServer [-port <port>]\n";
char tira_dll_absolute_path[] = "c:\\utils\\startup\\local\\acserver\\tira2.dll";

SOCKADDR saClient;
SOCKET rxSocket;

struct dsp * getDsp(void);
struct irCmd * cmdIdToIrCmd(int);

void activateSpeakerMenu(char *);
void ampInit(void);
void broadcastMsg(char *);
void delay(int);
void dispatchThread(void);
void initConsoleModeExitHandler(void);
void initDefaultInputSourceDspProgram(void);
void initSockets(void);
void initStartupDelay(int);
void irSend(char *);
void loadTiraDll(void);
void printErrorAndExit(char *);
void printIrCmd(struct irCmd *);
void printWinsockError(char *);
void setInputSourceDSP(void);
void setLFE(int);
void setSurroundProfile(int);
void setSurroundProfileByVolume(void);
void setSwitchMatrix(int);
void setVolume(char *);
