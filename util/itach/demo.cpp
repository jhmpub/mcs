// This utility uses the media control system common library to transmit
// ir coded message strings via TCP to the itach wifi to ir device
//
// compiles with gcc or msvc see build.bat for details
//
// 2016 May 23 jhm original creation
//

#include "common.h"
#include "socket.h"
#include "../../agent/remote/itach.h"
#include "demo.h"

int mode=VCR_MODE;
struct socketDescriptor sd = DEFAULT_SOCKET_DESCRIPTOR;
const char * szMode[REMOTE_MODES] = {"tv", "dvd", "vcr"};

int main(int argc, char **argv) {

   char c='\0';
   sd.host = "lvr-itach"; sd.port=4998; 
   sd.description = "itach"; sd.exit = TRUE;
    
   initWinsock();
   initClientThread(&sd);
   menu();
   tty_buffering(FALSE);

   while (c!='q') {
   
      c=getChar();
      
      switch (c) {
         case 'i':
            ++mode %= REMOTE_MODES;
            printf("\rmode: %s\n", szMode[mode]);    
            break;
         case '\n':
            break;
         case 'q':
            tty_buffering(TRUE);
            printf("\rExiting...\n");
            rxClientThreadExit(&sd);
            while(areSocketThreadsRunning())
               Sleep(500); 
            WSACleanup();
            break;
         default:
            sendMsg(getIrTx(c));   
      }
   }    
}


void sendMsg(struct irTx * irTx) {
   if (!irTx) {
      menu();
      return;
   }
   
   printf("\r");
   
   if (!irTx[mode].code) {
      printf("%s\n", irTx[mode].sz);
      return;
   }   
   
   sendMsg(irTx[mode].code, irTx[mode].sz);
   Sleep(600);
}


struct irTx * getIrTx(char c) {
   int i;
   for (i=0; i<sizeof(keymap)/sizeof(struct charToIrTx); i++) {
      if (keymap[i].c==c)
         return keymap[i].irTx;
   }
   return NULL;
}         


void menu() {
   int i;
   char sz[16];
   printf( "%-16siterate tv, dvd, vcr mode\n", "i");
   for (i=0; i<sizeof(keymap)/sizeof(struct charToIrTx); i++)
      printf("%-16s%s\n", keyToName(keymap[i].c, sz, sizeof(sz)), keymap[i].irTx[mode].sz);
   printf( "\ntype \'q\' to quit\n");
}
