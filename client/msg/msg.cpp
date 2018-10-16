// Transmit a TCP message to a media control server agent
//
// Usage: msg.exe ServerName <port> <msg> 
//
// 2016 Apr 7 jhm original creation
//

#include "socket.h"

int main(int argc, char **argv) {

   struct socketDescriptor sd = DEFAULT_SOCKET_DESCRIPTOR;
   
   if (argc != 4) {
      printf("\nUsage: msg.exe ServerName <port> <msg>\n");
      return 1;
   }
   
   sd.host = argv[1]; sd.port=atoi(argv[2]);

   initWinsock();
   
   if (establishConnection(&sd)) { 
      sendMsg(argv[3]);
      shutdownConnection(&sd);
   }   
   WSACleanup();
}

