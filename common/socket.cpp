// getnameinfo is only declared for Windows XP and later
#define _WIN32_WINNT 0x0501

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "common.h"
#include "socket.h"

// notifyList is globally used to manage tcp client connections needing status
// change notification updates
static struct socketDescriptor * activeSd;
static HANDLE serverThread=NULL;
static LinkedList * notificationList=NULL;
volatile const char * lock=NULL;

// create the server thread that listens on a bound socket
// n.b. current design only supports a single server instance
void createServerThread(SOCKET listeningSocket) {

   if (!notificationList)
      notificationList = new LinkedList();   
      
   if ((serverThread=CreateThread(NULL, // default security
         0,    // default stack size
         (LPTHREAD_START_ROUTINE)&socketListen,
         (LPVOID)listeningSocket, // thread parameter
         0,    // run thread immediately
         NULL)) // ignore thread id
        == NULL) {
      printErrorAndExit("createServerThread");
   }
}


//// socketListen /////////////////////////////////////////////////
// Spins forever waiting for connections.  For each one that comes
// in, create a thread to handle it and go back to waiting for
// connections.

DWORD socketListen(LPVOID param) {
   SOCKET listeningSocket = (SOCKET) param;
   sockaddr_in sinRemote;
   int nAddrSize = sizeof(sinRemote);
   LinkedList * nl;
   char host[256];
   SOCKET socket, ptr;
   struct socketDescriptor sd = { host, 0, &ptr, FALSE, FALSE, "client sd", NULL, NULL };

   while (1) {
      socket = accept(listeningSocket, (sockaddr*)&sinRemote, &nAddrSize);
      while(isLocked("socketListen"))
         Sleep(10);   // serialize msg processing
            
      if (socket==INVALID_SOCKET) {
         if (WSAGetLastError()==WSAEINTR) {
            // WSAEINTR is normal when process ends
            unlock();
            ExitThread(0);
         } 
         printf("socketListen: accept failed from %s:%u errno %u\n",
                inet_ntoa(sinRemote.sin_addr),
                ntohs(sinRemote.sin_port),
                WSAGetLastError());
         unlock();
      }
      else {
         HANDLE hThread;
         printf("\rServer accepted connection from %s on socket %d\n",
                lookupName(inet_ntoa(sinRemote.sin_addr), host, sizeof(host)), 
                socket);
         sd.port = ntohs(sinRemote.sin_port);
         *sd.socket = socket;
         if (hThread=CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&rxServerThread, (LPVOID)&sd, 0, NULL)) {
            // thread calls unlock() to prevent sd parameter corruption
            CloseHandle(hThread);
         } else {
            printErrorMsg("socketListen: CreateThread");
            unlock();
         }   
      }
   }
}


SOCKET socketBind(const char * host, int port) {
   SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
   if (sd != INVALID_SOCKET) {
      sockaddr_in sinInterface;
      sinInterface.sin_family = AF_INET;
      sinInterface.sin_addr.s_addr = lookupAddress(host);
      sinInterface.sin_port = htons(port);
      if (bind(sd, (sockaddr*)&sinInterface, 
              sizeof(sockaddr_in)) != SOCKET_ERROR) {
         listen(sd, SOMAXCONN);
         return sd;
      }
   }

   printErrorAndExit("socketBind");
   return 0;
}


// establish a client side connection
int establishConnection(struct socketDescriptor * sd) {

   // try to minimize duplicate log messages
   static char lastHostname[128];
   static int lastError;
      
   // Create a stream socket
   if (!sd->description) sd->description = "";
   *sd->socket = socket(AF_INET, SOCK_STREAM, 0);
   sockaddr_in sinRemote;
   sinRemote.sin_family = AF_INET;
   sinRemote.sin_addr.s_addr = lookupAddress(sd->host);
   sinRemote.sin_port = htons(sd->port);
   
   char hostname[256];
   lookupName(inet_ntoa(sinRemote.sin_addr), hostname, sizeof(hostname));

   if (*sd->socket==INVALID_SOCKET ||
       connect(*sd->socket, (sockaddr*)&sinRemote, sizeof(sockaddr_in))==SOCKET_ERROR) {
      if (strcmp(lastHostname, hostname) || lastError != WSAGetLastError()) {
         strncpy(lastHostname, hostname, sizeof(lastHostname));
         lastError = WSAGetLastError();
         if (WSAGetLastError() == WSAECONNREFUSED)
            printf("establishConnection %s: server %s refused connection request\n", szTime(), hostname);
         else if (WSAGetLastError() == WSAETIMEDOUT)
            printf("establishConnection %s: server %s connection request timed out\n", szTime(), hostname);
         else if (!sd->exit)   
            printErrorMsg("establishConnection");
      }
      
      *sd->socket = 0; 
      return FALSE;
   } else {
      lastError = NO_ERROR;
      lastHostname[0] = '\0';
   }   

   printf("Client established connection to %s on socket %d\n", 
           hostname, 
           *sd->socket);
           
   processServerConnect(sd->host);
   
   if (sd->reconnectMsg)
      sendMsg(sd, sd->reconnectMsg, sd->debugMsg);
           
   return TRUE;                 
}


// shutdown server and clients
void shutdownServer() {
      
   if (!serverThread)
      return;
     
   TerminateThread(serverThread, 0);
   CloseHandle(serverThread);
   serverThread=NULL;
   
   Sleep(500);
   
   LinkedList * nl;
   for (nl=notificationList->first(); nl->isNotLast(); nl=nl->next()) {
      shutdownConnection( (struct socketDescriptor *) nl->get() );
      nl->set(NULL);
   }
}         
      
      
// Try to gracefully shut down a connection using a given socket
void shutdownConnection(struct socketDescriptor * sd) {

   if (!sd || !*sd->socket)
      return;
      
   SOCKET socket = *sd->socket;
   *sd->socket=0;
   
   SOCKADDR sa;
   int salen=sizeof(sa);
   if (getpeername(socket, &sa, &salen) == SOCKET_ERROR) {   
      // calling to shutdown an unconnected socket is normal
      if (WSAGetLastError() != WSAENOTCONN)
         printErrorMsg("shutdownConnection getpeername");
      return;
   }         

   printf("Shutdown connection host %s socket %d\n", sd->host, socket);
   
   // Disallow any further data sends.  This will tell the other side
   // that we want to go away now.  If we skip this step, we don't
   // shut the connection down nicely.
   if (shutdown(socket, SD_SEND) == SOCKET_ERROR) {
      printErrorMsg("shutdownConnection shutdown");
      return;
   }

   // Receive any extra data still sitting on the socket.  After all
   // data is received, this call will block until the remote host
   // acknowledges the TCP control packet sent by the shutdown above.
   // Then we'll get a 0 back from recv, signalling that the remote
   // host has closed its side of the connection.
   char acReadBuffer[KBUFFERSIZE];
   while (1) {
      int afterBytes = recv(socket, acReadBuffer, KBUFFERSIZE, 0);
      if (afterBytes == SOCKET_ERROR) {
         if (WSAGetLastError() != WSAECONNRESET) {
            // ignore a recv error if the remote host forcibly closed the connection
            printErrorMsg("shutdownConnection recv");  
         }   
         return;
      }
      else if (afterBytes);
         // read and ignore extraneous data
      else
         break;
   }

   // Close the socket.
   if (closesocket(socket) == SOCKET_ERROR)
      printErrorMsg("shutdownConnection close socket");

   return;
}


//// lookupAddress /////////////////////////////////////////////////////
// Given an address string, determine if it's a dotted-quad IP address
// or a domain address.  If the latter, ask DNS to resolve it.  In
// either case, return resolved IP address.  If we fail, report the
// error and exit.

u_long lookupAddress(const char * host) {
   u_long nRemoteAddr = inet_addr(host);
   if (nRemoteAddr == INADDR_NONE) {
       // host isn't a dotted IP, so resolve it through DNS
       hostent * pHE = gethostbyname(host);
       if (pHE == 0) {
          printf("lookupAddress: failed gethostbyname(%s)\n", host);
          exit(INADDR_NONE);
       }
       nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
   }
   return nRemoteAddr;
}


//// lookupName ////////////////////////////////////////////////////////
// Given a dotted-quad IP address, perform a reverse DNS lookup to return
// the associated hostname.  If we fail, return the given ip string 

char * lookupName(const char * ip, char * sz, int szSize) {
   struct sockaddr_in sa;
   char * dotLan;
   char hostname[NI_MAXHOST];
   char servInfo[NI_MAXSERV];

   sa.sin_family = AF_INET;
   sa.sin_addr.s_addr = inet_addr(ip);
   sa.sin_port = htons(2000);

   int retval = getnameinfo((struct sockaddr *) &sa,                         
                            sizeof (struct sockaddr),                           
                            hostname, NI_MAXHOST, 
                            servInfo, NI_MAXSERV, 
                            NI_NUMERICSERV);

   if (retval) {
      printErrorMsg("lookupName");
      strncpy(sz, ip, szSize);
   } else {   
      if (dotLan=strstr(hostname, ".lan"))
         *dotLan = '\0';   // drop .lan on locally resolved hostnames 
      strncpy(sz, hostname, szSize);
   }
      
   return sz;
}


char * getHostname() {
    static char hostname[256] = "";
      
    if (!*hostname && gethostname(hostname, sizeof(hostname)))
       printErrorAndExit("getHostname");
       
    return hostname;
}       


char * initWinsock() {
    WORD wVersionRequested = MAKEWORD(1,1);
    WSADATA wsaData;
    int nRet;
    static char hostname[256];
    
    // Initialize WinSock and check version
    nRet = WSAStartup(wVersionRequested, &wsaData);
    if (wsaData.wVersion != wVersionRequested) {    
        printf("\ninitWinsock: wrong winsock version\n");
        exit(1);
    }
    
    return getHostname();   
}


void initTcpServer(const char * serverHost, int serverPort) {
    createServerThread(socketBind(serverHost,serverPort));
    printf("Host %s listening on port %d\n", serverHost, serverPort);               
}


void sendMsg(struct socketDescriptor * sd, const char * msg) {
   sendMsg(sd, msg, NULL);
}                     

void sendMsg(struct socketDescriptor * sd, const char * msg, const char * debugMsg) {
   printf("Send msg to host %s: %s\n", sd->host, debugMsg ? debugMsg : msg);
   
   int len = strlen(msg);
   char buf[len + 2];
   const char * crMsg;
   crMsg = msg[len-1]=='\r' ? msg : strcat(strcpy(buf, msg), "\r");

   if (send(*sd->socket, crMsg, strlen(crMsg), 0)==SOCKET_ERROR)
      printErrorMsg("sendMsg");
}


// direct a message to the rxServerThread client running processClientCmd
void sendMsg(const char * msg) {
   sendMsg(activeSd, msg);
}


// broadcast a message to all media control clients
void broadcastMsg(const char * msg) {

   LinkedList * nl;
   struct socketDescriptor * sd;
   int clientsRegistered=FALSE;
   
   int msgLen = strlen(msg);
   char crMsg[msgLen + 2];
   strcpy(crMsg, msg);
   if (crMsg[msgLen-1]!='\r') 
      strcat(crMsg, "\r");
   
   if (!notificationList)
      notificationList = new LinkedList();   

   for (nl=notificationList->first(); nl->isNotLast(); nl=nl->next()) {
      sd = (struct socketDescriptor *) nl->get();
      
      if (!sd || !sd->registered) {
         // don't bother sending updates to disconnected or unregistered clients
         continue;
      }
      
      clientsRegistered=TRUE;   
         
      if (send(*sd->socket, crMsg, strlen(crMsg), 0)==SOCKET_ERROR) {
            printErrorMsg("broadcastMsg");
      }
   }
   
   if (clientsRegistered)
      printf("Broadcasting msg: %s\n", msg);
}


void copySocketDescriptor(struct socketDescriptor * dst, struct socketDescriptor * src) {
   strcpy((char *)dst->host, src->host);
   dst->port = src->port;
   *dst->socket = *src->socket;
   dst->registered = src->registered;
   dst->exit = src->exit;
   strcpy((char *)dst->description, src->description);
}   
   

// receive and process client commands
DWORD rxServerThread(LPVOID param) {

   struct socketDescriptor * p = (struct socketDescriptor *) param;
   char host[strlen(p->host)+1], description[strlen(p->description)+1];
   char buffer[1024], msg[2048], * delim;
   int nBytes;
   LinkedList * nl;
   SOCKET socket;
   struct socketDescriptor sd = { host, 0, &socket, FALSE, FALSE, description };
   copySocketDescriptor(&sd, p);
   
   for (nl=notificationList->first(); nl->isNotLast(); nl=nl->next()) {
      if (nl->get()==NULL) {
         nl->set(&sd);
         break;
      }
   }
   if (nl->isLast())
      nl->add(&sd);

   msg[0]='\0';
   unlock();

   while(1) {
      nBytes=recv(socket, buffer, sizeof(buffer-1), 0);
      
      while(isLocked("rxServerThread"))
         Sleep(10);   // serialize msg processing
               
      buffer[nBytes]='\0';
      if (nBytes==0 || nBytes==SOCKET_ERROR) {
         if (nBytes==SOCKET_ERROR)
            printErrorMsg("rxServerThread");
         // close the connection
         for (nl=notificationList->first(); nl->isNotLast(); nl=nl->next()) {
            if (&sd==nl->get()) {
               processClientDisconnect(&sd);
               shutdownConnection(&sd);
               nl->set(NULL);
            }   
         }
         unlock();
         ExitThread(0);
      }
      else {
         // concatenate packets into a single carriage return terminated message
         // or process multiple carriage return terminated messages sequentially
      
         if (strlen(msg) + nBytes > sizeof(msg))
            msg[0]='\0';   // prevent an overrun

         strcat(msg, buffer);

         while (delim=strstr(msg, "\r")) {
            *delim='\0';
            
            activeSd=&sd;
            processClientMsg(&sd, msg);
            
            strcpy(msg, delim+1);
         }
         unlock();
      }
   }
}


// atomic compare and swap 
// if lock == NULL, lock = szFunc; return FALSE
// if lock != NULL, return TRUE
int isLocked(const char * szFunc) {
   if (InterlockedCompareExchange((volatile LONG *) &lock, lock!=NULL, FALSE))
      return TRUE;
   else {
      lock=szFunc;
      return FALSE;
   }      
}

void unlock() { lock=NULL; }


// receive and process agent/server notifications
DWORD rxClientThread(LPVOID param) {  
   char buffer[1024], msg[2048], * delim;
   int nBytes;
   struct socketDescriptor * sd = (struct socketDescriptor *) param;
   SOCKET * socket = sd->socket;
   
   if (!*socket)
      establishConnection(sd);

   while(1) {
      if (!*socket) {
         Sleep(10000);
         if (sd->exit)
            ExitThread(0);
         else  
            establishConnection(sd);
      } else {  
      
         nBytes=recv(*socket, buffer, sizeof(buffer-1), 0);
         
         while (isLocked("rxClientThread"))
            Sleep(10);   // serialize msg processing
             
         if (nBytes==0) {
            // graceful close
            shutdownConnection(sd);
         }   
         else if(nBytes==SOCKET_ERROR ) {
            // forcefully close
            printErrorMsg("rxClientThread");
            shutdownConnection(sd);
            if (!sd->exit)
               establishConnection(sd);
         } 
         else {
            // concatenate packets into a single carriage return terminated message
            // or process multiple carriage return terminated messages individually

            if (strlen(msg) + nBytes > sizeof(msg)) {
               // prevent an overrun
               msg[0]='\0';
            }

            buffer[nBytes]='\0';
            strcat(msg, buffer);

            while (delim=strstr(msg, "\r")) {
               *delim='\0';
               processAgentNotification(sd->host, msg);
               strcpy(msg, delim+1);
            }   
         }
         unlock();
      }
   }
}


void clrReconnectMsg(struct socketDescriptor * sd) {
   sd->reconnectMsg=NULL;
   sd->debugMsg=NULL;
}


void setReconnectMsg(struct socketDescriptor * sd, const char * msg, const char * debugMsg) {
   sd->reconnectMsg=msg;
   sd->debugMsg=debugMsg;
}

   
void rxClientThreadExit(struct socketDescriptor * sd) {
   sd->exit=TRUE;
   shutdownConnection(sd);
}   


void printErrorAndExit(const char * func) {
   DWORD dw = GetLastError(); 
   printErrorMsg(func);
   ExitProcess(dw); 
}


void printErrorMsg(const char * func) {

   LPVOID lpMsgBuf;
   DWORD dw = GetLastError(); 

   FormatMessage(
       FORMAT_MESSAGE_ALLOCATE_BUFFER | 
       FORMAT_MESSAGE_FROM_SYSTEM |
       FORMAT_MESSAGE_IGNORE_INSERTS,
       NULL,
       dw,
       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
       (LPTSTR) &lpMsgBuf,
       0, NULL );
       
   if (dw==WSAECONNRESET) 
      printf("%s: remote host forcibly closed the connection\n", func);
   else if (dw==WSAECONNREFUSED)
          printf("%s: server refused connection request\n", func);
   else    
      printf("\n%s error(%d): %s\n", func, dw, lpMsgBuf);    

   LocalFree(lpMsgBuf);
}


// return a string formatted with the current HH.MM.SS
char * szTime() {
   SYSTEMTIME tm;
   static char szTm[9];
   GetLocalTime(&tm);
   snprintf(szTm, sizeof(szTm), "%02d.%02d.%02d", tm.wHour, tm.wMinute, tm.wSecond);
   return szTm;
}   


#ifdef NO_MCS
void processAgentNotification(const char * server, char * msg) {
   printf("processAgentNotification from host %s: %s\n", server, msg);
}
void processServerConnect(const char *) {}
#endif

#ifdef NO_COMMON
void processClientDisconnect(struct socketDescriptor * sd) {}
void processClientMsg(struct socketDescriptor * sd, char * msg) {
   printf("processClientMsg from host %s: %s\n", sd->host, msg);
}   
#endif


