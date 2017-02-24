// This utility uses the media control system common library to transmit
// ir coded message strings via TCP to the itach wifi to ir device
//
// compiles with cygwin gcc - http://cygwin.com
// see batch.bat for details
//
// 2016 May 23 jhm original creation
//

#include <winsock2.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "../../common/common.h"
#include "../../common/socket.h"
#include "../../agent/remote/itach_codes.h"
#include "demo.h"

void initClientThread(struct socketDescriptor *);
void help(void);
void sendMsg(struct irTx *);
struct irTx * getIrTx(char);

int mode=TV_MODE;
struct socketDescriptor sd;
const char * szMode[REMOTE_MODES] = {"tv", "dvd", "vcr"};

int main(int argc, char **argv) {

   char c, offset;
   int prompt=TRUE;
   int state=0, escape=0;
   SOCKET socket;
   ZeroMemory(&sd, sizeof(sd));
   sd.host = "lvr-itach"; sd.port=4998; sd.socket=&socket;
    
   initWinsock();
   if (establishConnection(&sd))
      initClientThread(&sd);
   else {
      WSACleanup();
      exit(1);
   } 
   
   help();
   stty_unbuffered();

   while (1) {
   
      prompt ? printf(">") : prompt=TRUE;
      c=tolower(getchar());
      state>escape ? escape=state : state=escape=0;
      
      switch (c) {
         case 'i':
            ++mode %= REMOTE_MODES;
            printf("\rmode: %s\n", szMode[mode]);    
            break;
         case '\x61':
         case '\x62':
         case '\x63':
         case '\x64':
            offset += 0x20;
         case '\x41':
         case '\x42':
         case '\x43':
         case '\x44':
            if (escape)
               sendMsg(arrow[c-'\x41'-offset].irTx);
            else
               sendMsg(getIrTx(c));
            offset=0;   
            break;   
         case '\x1b':
            state++;
         case '\x5b':
            if (state && c=='\x5b')
               state++;
            if (state==escape)   
               sendMsg(getIrTx(c));
            prompt=FALSE;
         case '\n':
            break;
         case 'q':
            stty_buffered();
            printf("\rExiting...\n");
            shutdownConnection(&sd);
            WSACleanup();
            exit(0);
         case 'p':
            if (mode==TV_MODE) {
               // panasonic TVs seem to ignore subsequent power ir commands 
               // even after a significant period of time.  Sending an 
               // arbitrarily different ir command first makes the power
               // command reliable so send a return ir command first :( 
               sendMsg(getIrTx('r'));
            }      
         default:
            //printf("*** 0x%x\n", c);
            sendMsg(getIrTx(c));   
      }
   }    
}


void sendMsg(struct irTx * irTx) {
   if (!irTx) {
      help();
      return;
   }
   
   printf("\r");
   
   if (!irTx[mode].code) {
      printf("%s\n", irTx[mode].sz);
      return;
   }   
   
   sendMsg(&sd, irTx[mode].code, irTx[mode].sz);
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


void initClientThread(struct socketDescriptor * sd) {

   if (CreateThread(NULL,  // default security
                    0,     // default stack size
                    (LPTHREAD_START_ROUTINE)&rxClientThread,
                    sd,    // thread parameter
                    0,     // run thread immediately
                    NULL)  // ignore thread id
        == NULL) {
      char description[strlen("init") + strlen(sd->description)+1];
      printErrorAndExit((const char *)strcat(strcpy(description,"init"),sd->description));
   }
} 


void help() {
   int i;
   printf( "\rup arrow\t%s menu up\n", szMode[mode]);
   printf( "dn arrow\t%s menu down\n", szMode[mode]);
   printf( "lt arrow\t%s menu left\n", szMode[mode]);
   printf( "rt arrow\t%s menu right\n", szMode[mode]);
   printf( "i\t\titerate tv, dvd, vcr mode\n");
   for (i=0; i<sizeof(keymap)/sizeof(struct charToIrTx); i++)
      printf("%c\t\t%s\n", keymap[i].c, keymap[i].irTx[mode].sz);
   printf( "\ntype \'q\' to quit\n");
}
