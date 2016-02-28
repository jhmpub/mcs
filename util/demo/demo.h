#define ESCAPE     -32
#define ARROW_UP    72 
#define ARROW_DOWN  80 
#define ARROW_LEFT  75
#define ARROW_RIGHT 77

char tira_dll_absolute_path[] = 
   "\\\\georgia\\backup\\install\\drivers\\interface\\usb\\tira\\sdk\\Tira2.dll";

int __stdcall irCmdCallback(const char *);
void help(void);
struct irMsg cmd2irMsg(int);
void sendIrCmd(int);
void sendIrCode(const unsigned char *, int, char *);
void stty_buffered(void);
void stty_unbuffered(void);
void loadTiraDLL(void);

static struct termios old_tio, new_tio;
static char captureActive;
static const unsigned char* txData;
static HMODULE handle = 0;


