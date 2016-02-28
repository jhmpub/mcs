// This utility is a standalone application that will transmit
// and capture IR commands using the Tira device.
//
// compiles with cygwin gcc - http://cygwin.com
// gcc -o demo demo.c
//
// 2004 Aug 1 jhm original creation
//

#include <winsock2.h>
#include <unistd.h>
#include <termios.h>
#include "../../common/tira.h"
#include "../../common/xantech_codes.h"
#include "../../common/yamaha_codes.h"
#include "../../common/common.h"
#include "../../common/extern.h"
#include "demo.h"

int main(int argc, char ** argv) {

   const unsigned char * Data = (const unsigned char *) GlobalAlloc(GPTR, 1024);
   txData = Data;
   int error, DataSize = 0;
   char buff[32];
   int res = 0, i;
   char c;
   
   loadTiraDll();
   printf("Library loaded\n");
   tira_init();
   
   // default to port 4 unless specified otherwise on the command line
   int port = (argc==2 && atoi(argv[1])) ? atoi(argv[1]) : DEFAULT_COM_PORT;
   
   printf("Tira activat%s on com port %d\n", 
          (error=tira_start(port-1)) ? "ion failed" : "ed", port);
   if (error) { printf("\nUsage: demo [port]\n"); exit(1); }
   
   help();
   
   stty_unbuffered();

   while (1) {

      Sleep(50);

      if ( captureActive ) {
      
         res = tira_get_captured_data(&Data, &DataSize);
         
         if ( Data != 0 && DataSize != 0 ) {
            printf("IR Code captured!\n");
            printf("DataSize: %d\n", DataSize);
            int i;
            for (i=0; i<DataSize; i++) {
                printf("0x%02X, ", Data[i]);
                if (((i+1) % 10) == 0) printf("\n");
            }
            res = tira_cancel_capture();
            if (res==0) printf("\nCapture deactivated\n");
            printf("\n");
            captureActive = FALSE;
         }
         
      } else {
      
         printf(">");
         c=getchar();
         printf("\n");

         switch (c) {
            case 'a':
                  res = tira_start_capture();
                  if (res==0) printf("Playback capture activated\n");
                  captureActive = (res == 0);
                  break;
                  
            case 'b':
                  res = tira_set_handler(irCmdCallback);
                  if (res==0) printf("Callback capture activated\n");
                  break;
                  
            case 'c':
                  res = tira_cancel_capture();
                  if (res==0) printf("Capture deactivated\n");
                  captureActive = FALSE;
                  break;
         
            case '0':
                  sendIrCode(Data, DataSize, "learned");
                  break;
            case 'i':
                  sendIrCmd(POWER_ON);
                  break;
            case 'o':
                  sendIrCmd(POWER_OFF);
                  break;
            case 'f':
                  sendIrCmd(FM);
                  break;
            case 'g':
                  sendIrCmd(GEORGIA);
                  break;
            case 'j':
                  sendIrCmd(JIMSON);
                  break;
            case 't':
                  sendIrCmd(TV);
                  break;
            case 'v':
                  sendIrCmd(DVD);
                  break;
            case 'y':
                  sendIrCmd(VIDEO_AUX);
                  break;
            case 'u':
                  for (i=0;i<8;i++)
                      sendIrCmd(MASTER_VOLUME_UP);
                  break;
            case 'd':
                  for (i=0;i<8;i++)
                      sendIrCmd(MASTER_VOLUME_DOWN);
                  break;
            case 'm':
                  sendIrCmd(MUTE_TOGGLE);
                  break;
            case ')':
                  sendIrCmd(CHANNEL_A_TO_FRONT_SPEAKERS);
                  break;
            case '(':
                  sendIrCmd(CHANNEL_A_TO_AB_SWITCH);
                  break;
            case ']':
                  sendIrCmd(REAR_SPEAKERS_FROM_AB_SWITCH);
                  break;
            case '[':
                  sendIrCmd(REAR_SPEAKERS_FROM_REAR_CHANNEL);
                  break;
            case '+':
                  sendIrCmd(CHANNEL_A_RELAY_ON);
                  break;
            case '-':
                  sendIrCmd(CHANNEL_A_RELAY_OFF);
                  break;
            case '>':
                  sendIrCmd(CHANNEL_B_SPEAKERS_ON);
                  break;
            case '<':
                  sendIrCmd(CHANNEL_B_SPEAKERS_OFF);
                  break;
            case '}':
                  sendIrCmd(SUBWOOFER_ON);
                  break;
            case '{':
                  sendIrCmd(SUBWOOFER_OFF);
                  break;
            case ':':
                  sendIrCmd(DSP_DOLBY_NORMAL);
                  break;
            case ';':
                  sendIrCmd(DSP_EFFECT_TOGGLE);
                  break;
            case '1':
                  sendIrCmd(FM1);
                  break;
            case '2':
                  sendIrCmd(FM2);
                  break;
            case '3':
                  sendIrCmd(FM3);
                  break;
            case '4':
                  sendIrCmd(FM4);
                  break;
            case '5':
                  sendIrCmd(FM5);
                  break;
            case '6':
                  sendIrCmd(FM6);
                  break;
            case '7':
                  sendIrCmd(FM7);
                  break;
            case '8':
                  sendIrCmd(FM8);
                  break;
            case 's':
                  sendIrCmd(MENU_SETTINGS);
                  break;
            case 'l':
                  sendIrCmd(MENU_EFFECT_SPEAKER_LEVEL);
                  break;
            case ESCAPE:
                  c = getchar();
                  
                  switch (c) {
                      case ARROW_UP:
                          sendIrCmd(MENU_UP);
                          break;
                      case ARROW_DOWN:
                          sendIrCmd(MENU_DOWN);
                          break;
                      case ARROW_RIGHT:
                          sendIrCmd(MENU_RIGHT);
                          break;
                      case ARROW_LEFT:
                          sendIrCmd(MENU_LEFT);
                          break;
                      default:
                          help();    
                  }        
                          
                  break;      
            case '\n':
                  break;
            case 'q':
            case 'Q':
                  stty_buffered();
                  printf("Exiting...\n");
                  return 0;
            default:
                  help();
         }
     }    
     
     if (res)
        printf("\nLast function call failed!\n");
            
         
   }
   return 0;
}


void sendIrCode(const unsigned char * irCodes, int size, char * irCodeName) {

    char buff[32];
    int res = -1;
    int i;

    memcpy((HGLOBAL)txData, irCodes, size);
    
    printf("%d\n",size);
   
    for (i=0; i<size; i++ ) {
        printf("0x%02X, ", irCodes[i]);
        if (((i+1) % 10) == 0) printf("\n");
    }
    
    printf("\n");
    
    res = tira_transmit(0, -1, txData, size);

    if ( res == 0 ) {
        printf("\nIR %s code transmitted\n\n", irCodeName);
    }    

    captureActive = FALSE;
}               


void sendIrCmd(int cmd) {
    struct irCmd * irCmd = cmdIdToIrCmd(cmd);
    sendIrCode(irCmd->codes, irCmd->size, irCmd->sz);
}               


void stty_unbuffered() {
    // get a copy of the stdin settings and disable the buffering flag
	tcgetattr(STDIN_FILENO, &old_tio);
	new_tio=old_tio;
	new_tio.c_lflag &=(~ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}


void stty_buffered() {
	// restore the former stdin settings
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}    


int __stdcall irCmdCallback(const char * irCmd) {
    printf("%s\n", irCmd);
    return 0;
}    


void help() {
            
   printf( "a\t\t activates capture mode\n");
   printf( "b\t\t activate the capture callback method\n");
   printf( "c\t\t cancels capture mode\n");
   printf( "0\t\t transmit the dynamic IR code\n");
   printf( "i\t\t transmit %s\n", SZ_POWER_ON);
   printf( "o\t\t transmit %s\n", SZ_POWER_OFF);
   printf( "f\t\t transmit %s\n", SZ_FM);
   printf( "g\t\t transmit %s\n", SZ_GEORGIA);
   printf( "j\t\t transmit %s\n", SZ_JIMSON);
   printf( "t\t\t transmit %s\n", SZ_TV);
   printf( "v\t\t transmit %s\n", SZ_DVD);
   printf( "y\t\t transmit %s\n", SZ_VIDEO_AUX);
   printf( "u\t\t transmit %s\n", SZ_MASTER_VOLUME_UP);
   printf( "d\t\t transmit %s\n", SZ_MASTER_VOLUME_DOWN);
   printf( "m\t\t transmit %s/%s toggle\n", SZ_MUTE_ON, SZ_MUTE_OFF);
   printf( ")\t\t set %s\n", SZ_CHANNEL_A_TO_FRONT_SPEAKERS); 
   printf( "(\t\t set %s\n", SZ_CHANNEL_A_TO_AB_SWITCH); 
   printf( "]\t\t set %s\n", SZ_REAR_SPEAKERS_FROM_AB_SWITCH); 
   printf( "[\t\t set %s\n", SZ_REAR_SPEAKERS_FROM_REAR_CHANNEL); 
   printf( "+\t\t turn %s\n", SZ_CHANNEL_A_RELAY_ON); 
   printf( "-\t\t turn %s\n", SZ_CHANNEL_A_RELAY_OFF); 
   printf( ">\t\t turn %s\n", SZ_CHANNEL_B_SPEAKERS_ON); 
   printf( "<\t\t turn %s\n", SZ_CHANNEL_B_SPEAKERS_OFF); 
   printf( "}\t\t turn %s\n", SZ_SUBWOOFER_ON); 
   printf( "{\t\t turn %s\n", SZ_SUBWOOFER_OFF); 
   printf( ";\t\t %s\n", SZ_DSP_EFFECT_TOGGLE);
   printf( ":\t\t %s\n", SZ_DSP_DOLBY_NORMAL);
   printf( "1\t\t %s\n", SZ_FM1);
   printf( "2\t\t %s\n", SZ_FM2);
   printf( "3\t\t %s\n", SZ_FM3);
   printf( "4\t\t %s\n", SZ_FM4);
   printf( "5\t\t %s\n", SZ_FM5);
   printf( "6\t\t %s\n", SZ_FM6);
   printf( "7\t\t %s\n", SZ_FM7);
   printf( "8\t\t %s\n", SZ_FM8);
   printf( "l\t\t enter OSD %s\n", SZ_MENU_EFFECT_SPEAKER_LEVEL);
   printf( "s\t\t enter OSD %s\n", SZ_MENU_SETTINGS);
   
   printf( "arrow up\t navigate OSD %s\n", SZ_MENU_UP);
   printf( "arrow down\t navigate OSD %s\n", SZ_MENU_DOWN);
   printf( "arrow right\t navigate OSD %s\n", SZ_MENU_RIGHT);
   printf( "arrow left\t navigate OSD %s\n", SZ_MENU_LEFT);
   printf( "q\t\t Quit\n\n");
}

