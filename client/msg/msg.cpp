// This is a simple utility that transmits TCP messages
// to the acServer.exe daemon.
//
// Usage: msg.exe ServerName <msg> 
//
// 2016 Apr 7 jhm original creation
//

#include <winsock2.h>
#include <stdio.h>
#include "../../common/socket.h"

int main(int argc, char **argv) {

   SOCKET socket;
   struct socketDescriptor sd;
   
   if (argc != 4) {
      printf("\nUsage: msg.exe ServerName <port> <msg>\n");
      return 1;
   }
   
   ZeroMemory(&sd, sizeof(sd));
   sd.host = argv[1];  sd.port=atoi(argv[2]); sd.socket=&socket;

   initWinsock();
   
   if (establishConnection(&sd)) { 
      sendMsg(&sd, argv[3]);
      shutdownConnection(&sd);
   }   
   WSACleanup();
}

