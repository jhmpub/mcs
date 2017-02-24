#define ESCAPE     -32
#define ARROW_UP    72 
#define ARROW_DOWN  80 
#define ARROW_LEFT  75
#define ARROW_RIGHT 77

int __stdcall irCmdCallback(const char *);
void help(void);
struct irMsg cmd2irMsg(int);
void sendIrCmd(int);
void sendIrCmd(struct tiraCmd *);
void loadTiraDLL(void);

static char captureActive, callbackActive;


