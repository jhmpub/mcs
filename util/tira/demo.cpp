// This utility uses the media control system common library to transmit
// and capture IR commands using the Tira device.
//
// compiles with mingw gcc - http://mingw.org
// see build.bat and media_control_system.pdf for details
//
// 2004 Aug 1 jhm original creation
//

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "tira_dll.h"
#include "demo.h"


int main(int argc, char ** argv) {

   struct tiraCmd captured = {UNDEFINED, "learned", &undefined, 0, NULL};
   captured.codes = (const unsigned char *) GlobalAlloc(GPTR, 1024);
   int error;
   char buff[32];
   int res = 0, i;
   char c='\0';
   
   // use DEFAULT_TIRA_COM_PORT unless specified otherwise on the command line
   int port = (argc==2 && atoi(argv[1])) ? atoi(argv[1]) : DEFAULT_TIRA_COM_PORT;
   error=initTiraRemoteControl(port);
   if (error) { printf("\nUsage: demo [port]\n"); exit(1); }
   
   printf("Tira set callback handler %s\n\n", 
          (error=tira_set_handler(irCmdCallback)) ? "failed" : "succeeded");
   if (error) exit(1);
                  
   menu();
   
   tty_buffering(FALSE);

   while (c!='q') {

      Sleep(50);

      if ( captureActive ) {
      
         res = tira_get_captured_data(&captured.codes, &captured.size);
         
         if ( captured.codes != 0 && captured.size != 0 ) {
            printf("IR Code captured!\n");
            printf("Captured size: %d\n", captured.size);
            int i;
            for (i=0; i<captured.size; i++) {
                printf("0x%02X, ", captured.codes[i]);
                if (((i+1) % 10) == 0) printf("\n");
            }
            res = tira_cancel_capture();

            printf("%s\n", (res) ? "\nLast function call failed!\n\n" : 
                                   "\nCapture deactivated\n\n");
            captureActive = FALSE;
         }
         
      } else {
      
         printf(">");
         c=getChar();
         printf("\n");

         switch (c) {
            case 'a':
                callbackActive = FALSE;
                res = tira_start_capture();
                if (res==0) printf("Playback capture activated\n");
                captureActive = (res == 0);
                break;
            case 'b':
                callbackActive = TRUE;
                printf("Callback capture activated\n");
                break;
            case 'c':
                res = tira_cancel_capture();
                if (res==0) printf("Capture deactivated\n");
                callbackActive = captureActive = FALSE;
                break;
            case '!':
                if (captured.size)
                   sendIrCmd(&captured);
                break;
            case '\n':
                break;
            case 'q':
                tty_buffering(TRUE);
                printf("Exiting...\n");
                break;
            default:
                sendIrCmd(c); 
         }
      }    
   }
}


void sendIrCmd(char key) {
    int tiraCmdId = keyToTiraCmdId(key);
    (tiraCmdId==UNDEFINED) ? menu() : 
                             sendIrCmd(tiraCmdIdToTiraCmd(tiraCmdId));      
}


void sendIrCmd(struct tiraCmd * cmd) {    
    int i;
    printf("Code size: %d\n",cmd->size);
   
    for (i=0; i<cmd->size; i++ ) {
       printf("0x%02X, ", cmd->codes[i]);
       if (((i+1) % 10) == 0) printf("\n");
    }
    
    printf("\n\nIR %s code %s\n\n", cmd->sz, 
       tiraTransmit(cmd) ? "transmission failed" : "transmitted");
}               


int __stdcall irCmdCallback(const char * szCmd) {
    if (callbackActive)
       printf("%s\n", szCmd);
    return 0;
}    


void menu() {
   int i;
   char sz[16];
   for (i=0; i<sizeof(tiraCmdMap)/sizeof(struct tiraCmdMap); i++)
      printf("%s\t\t%s\n", keyToName(tiraCmdMap[i].key, sz, sizeof(sz)), tiraCmdMap[i].description);
   printf("\n");   
} 


int keyToTiraCmdId(const char key) {
   int i;
   for (i=0; i<sizeof(tiraCmdMap)/sizeof(struct tiraCmdMap); i++) {
      if (tiraCmdMap[i].key==key)
         return tiraCmdMap[i].id;
   }
   return UNDEFINED;
}         


