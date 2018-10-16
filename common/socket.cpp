// getnameinfo is only declared for Windows XP and later
#define _WIN32_WINNT 0x0501

#include "socket.h"

// notifyList is globally used to manage tcp client connections needing status
// change notification updates
static struct socketDescriptor serverListenSd = DEFAULT_SOCKET_DESCRIPTOR;
static struct socketDescriptor * singleThreadSd=NULL;
static LinkedList * serverList=NULL;
static LinkedList * clientList=NULL;

// create the server thread that listens on a bound socket
// n.b. current design only supports a single server instance
HANDLE createServerListenThread(SOCKET listeningSocket) {

   static HANDLE serverThread=NULL;
   
   if (serverThread)
      return serverThread;
   
   if (!serverList)
      serverList = new LinkedList();   
      
   if ((serverThread=CreateThread(NULL, // default security
         0,    // default stack size
         (LPTHREAD_START_ROUTINE)&serverListenThread,
         (LPVOID)listeningSocket, // thread parameter
         0,    // run thread immediately
         NULL)) // ignore thread id
        == NULL) {
      printErrorAndExit("createServerListenThread");
   }
   
   return serverThread;
}


// Spins forever waiting for connections.  For each one that comes
// in, create a thread to handle it and go back to waiting for
// connections
DWORD serverListenThread(LPVOID arg) {
   SOCKET serverSocket = (SOCKET) arg;
   sockaddr_in sinRemote;
   int nAddrSize = sizeof(sinRemote);
   char host[256];
   SOCKET socket;
   struct socketDescriptor * serverSd;
   struct socketDescriptor sd = DEFAULT_SOCKET_DESCRIPTOR;
   sd.description = "client sd";

   while (1) {
      socket = accept(serverSocket, (sockaddr*)&sinRemote, &nAddrSize);
      
      if (serverListenSd.exit) {
         CloseHandle(serverListenSd.hThread);
         serverListenSd.hThread=NULL;
         printq("serverListenThread thread exit\n");
         ExitThread(0);
      }   
         
      if (socket==INVALID_SOCKET) {
         if (WSAGetLastError()==WSAEINTR) {
            // WSAEINTR is normal when process ends
            CloseHandle(serverListenSd.hThread);
            serverListenSd.hThread=NULL;
            printq("serverListenThread WSAEINTR thread exit\n");
            ExitThread(0);
         } 
         printq("serverListenThread: accept failed from %s:%u errno %u\n",
                inet_ntoa(sinRemote.sin_addr),
                ntohs(sinRemote.sin_port),
                WSAGetLastError());
      }
      else {
         HANDLE hThread;
         printq("\rServer accepted connection from %s on socket %d\n",
                lookupName(inet_ntoa(sinRemote.sin_addr), host, sizeof(host)), 
                socket);
                
         sd.port = ntohs(sinRemote.sin_port);
         sd.socket = socket;
         sd.connected = TRUE;
         serverSd = allocateServerSocketDescriptor(&sd);
         singleThreadSd = serverSd;
         if ( !(serverSd->hThread=CreateThread(0, 0, 
                                               (LPTHREAD_START_ROUTINE) &rxServerThread, 
                                               (LPVOID) serverSd, 
                                               0, NULL)) ) {
            printErrorMsg("serverListenThread: create rxServerThread");
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


socketDescriptor * allocateServerSocketDescriptor(socketDescriptor * p) {
   LinkedList * l;
   struct socketDescriptor * serverSd;
   
   char * host = (char *) malloc(strlen(p->host)+1);
   char * description = (char *) malloc(strlen(p->description)+1);

   for (l=serverList->first(); l->isNotLast(); l=l->next()) {
      serverSd = (struct socketDescriptor *) l->get();
      if (serverSd->hThread==NULL)
         break;   // socket descriptor is free for reuse
   }
   if (l->isLast()) {
      serverSd = (socketDescriptor *) malloc(sizeof(struct socketDescriptor)); 
      l->add(serverSd);
   }
   
   serverSd->host = host;
   serverSd->description = description;
   copySocketDescriptor(serverSd, p);
   
   return serverSd;
}      


void copySocketDescriptor(struct socketDescriptor * dst, struct socketDescriptor * src) {
   strcpy((char *)dst->host, src->host);
   dst->port = src->port;
   dst->socket = src->socket;
   dst->hThread = src->hThread;
   dst->lastError = src->lastError;
   dst->connected = src->connected;
   dst->registered = src->registered;
   dst->exit = src->exit;
   dst->reconnectMsg = src->reconnectMsg;
   strcpy((char *)dst->description, src->description);
}   
   

// establish a client side connection
int establishConnection(struct socketDescriptor * sd) {

   // create a stream socket
   if (!sd->description) sd->description = "";
   sd->socket = socket(AF_INET, SOCK_STREAM, 0);
   sockaddr_in sinRemote;
   sinRemote.sin_family = AF_INET;
   if ((sinRemote.sin_addr.s_addr = lookupAddress(sd->host)) == INADDR_NONE)
      return FALSE;
   sinRemote.sin_port = htons(sd->port);
   
   char hostname[256];
   lookupName(inet_ntoa(sinRemote.sin_addr), hostname, sizeof(hostname));
   

   if (sd->socket==INVALID_SOCKET ||
       connect(sd->socket, (sockaddr*)&sinRemote, sizeof(sockaddr_in))==SOCKET_ERROR) {
      if (sd->lastError != WSAGetLastError()) {
         sd->lastError = WSAGetLastError();
         if (sd->lastError == WSAECONNREFUSED)
            printq("establishConnection %s: server %s refused connection request\n", szTime(), hostname);
         else if (sd->lastError == WSAETIMEDOUT)
            printq("establishConnection %s: server %s connection request timed out\n", szTime(), hostname);
         else if (!sd->exit)   
            printErrorMsg("establishConnection");
      }
      
      if (sd->socket != INVALID_SOCKET) {
         closesocket(sd->socket);
         sd->socket = INVALID_SOCKET;
      }   
      
      sd->connected=FALSE;
      return FALSE;
   } else {
      sd->lastError = NO_ERROR;
   }   

   printq("Client established connection to %s on socket %d\n", 
           hostname, 
           sd->socket);
           
   processServerConnect(sd->host);
   
   if (sd->reconnectMsg)
      sendMsg(sd, sd->reconnectMsg, sd->debugMsg);
   
   singleThreadSd = sd;
   sd->connected=TRUE;        
   return TRUE;                 
}


// shutdown server and clients
void shutdownServer() {
      
   if (serverListenSd.exit || !serverList)
      return;
    
   serverListenSd.exit = TRUE;
   
   if (serverListenSd.socket != INVALID_SOCKET) {
      closesocket(serverListenSd.socket);
      serverListenSd.socket = INVALID_SOCKET;
   }     
   
   LinkedList * l;
   struct socketDescriptor * sd;
   for (l=serverList->first(); l->isNotLast(); l=l->next()) {
      sd = (struct socketDescriptor *) l->get();
      sd->exit = TRUE;
      shutdownConnection(sd);
   }
}         
      
      
// Try to gracefully shut down a connection using a given socket
void shutdownConnection(struct socketDescriptor * sd) {

   sd->connected = FALSE;
   
   if (sd->socket==INVALID_SOCKET)
      return;
      
   SOCKET socket = sd->socket;
   sd->socket=INVALID_SOCKET;
   
   SOCKADDR sa;
   int salen=sizeof(sa);
   if (getpeername(socket, &sa, &salen) == SOCKET_ERROR) {   
      // calling to shutdown an unconnected socket is normal
      if (WSAGetLastError() != WSAENOTCONN)
         printErrorMsg("shutdownConnection getpeername");
   }         

   printq("Shutdown connection host %s socket %d\n", sd->host, socket);
   
   // Disallow any further data sends.  This will tell the other side
   // that we want to go away now.  If we skip this step, we don't
   // shut the connection down nicely.
   if (shutdown(socket, SD_SEND) == SOCKET_ERROR) {
      printErrorMsg("shutdownConnection shutdown");
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
         closesocket(socket);   
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


// Given an address string, determine if it's a dotted-quad IP address
// or a domain address.  If the latter, ask DNS to resolve it.  In
// either case, return resolved a IP address.  On failure report the
// error and exit
u_long lookupAddress(const char * host) {
   u_long nRemoteAddr = inet_addr(host);
   if (nRemoteAddr == INADDR_NONE) {
       // host isn't a dotted IP, so resolve it through DNS
       hostent * pHE = gethostbyname(host);
       if (pHE == 0) {
          const char * format = "lookupAddress failed gethostbyname(%s)";
          char * szErr = (char *) malloc(strlen(format) + strlen(host) + 1);
          sprintf(szErr, format, host);
          printErrorAndExit(szErr);
       }
       nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
   }
   return nRemoteAddr;
}


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


void initTcpServer(const char * host, int port) {
    serverListenSd.host = host;
    serverListenSd.port = port;
    serverListenSd.socket = socketBind(host, port);
    serverListenSd.hThread = createServerListenThread(serverListenSd.socket);
    serverListenSd.exit = FALSE;
    serverListenSd.description = "TCP Server";
    printq("Host %s listening on port %d\n", host, port);               
}


// generic routine to create a thread that maintains a connection 
// for receiving server messages
void initClientThread(struct socketDescriptor * sd) {
   int connected;
   
   if ((connected=establishConnection(sd)) || !sd->exit) {
      // start rx thread if connected or if thread should retry (not exit)
      sd->exit=FALSE;   
      sd->hThread = CreateThread(
         NULL,      // default security
         0,         // default stack size
         (LPTHREAD_START_ROUTINE)&rxClientThread,
         sd,        // thread parameter
         0,         // run thread immediately
         NULL);  // ignore thread id
   }      
       
   if ((!connected && sd->exit) || !sd->hThread) {
      const char * init = "initClientThread ";
      if (!sd->description) sd->description = sd->host;
      char * description = (char *) malloc(strlen(init) + strlen(sd->description)+1);
      printErrorAndExit((const char *)strcat(strcpy(description,init),sd->description));
   }
} 


// override the default debug message when calling sendMsg() 
void sendMsg(struct socketDescriptor * sd, const char * msg, const char * debugMsg) {
   if (!sd) {
      printq("sendMsg: NULL socketDescriptor\n");
      return;
   }   
   sd->debugMsg = debugMsg;
   sendMsg(sd, msg);
}

void sendMsg(struct socketDescriptor * sd, const char * msg) {
   if (!sd) {
      printq("sendMsg: NULL socketDescriptor\n");
      return;
   }   
      
   printq("Send msg to host %s: %s\n", sd->host, sd->debugMsg ? sd->debugMsg : msg);
   
   int len = strlen(msg);
   char * buf = (char *) malloc(len + 2);
   const char * crMsg;
   crMsg = msg[len-1]=='\r' ? msg : strcat(strcpy(buf, msg), "\r");

   if (send(sd->socket, crMsg, strlen(crMsg), 0)==SOCKET_ERROR &&
       WSAGetLastError()!=WSAECONNABORTED) {
       // an aborted connection error is normal on a half closed connection
      printErrorMsg("sendMsg");
   }   
   free(buf);
}


#ifndef MCS
// sendMsg(const char *) is not valid in a multithreaded, multisocket scenario
// Multithreaded applications must associate message streams with their 
// socketDescriptor to explicitly call 
// sendMsg(struct socketDescriptor *, const char *).  Media Control System
// applications currently uses QueuedMsg objects.

// for simplicity in a single threaded, single connection scenario,
// send a message on the only existing socket
void sendMsg(const char * msg) {
   sendMsg(singleThreadSd, msg);
}

// override the default debug message when calling sendMsg() 
void sendMsg(const char * msg, const char * debugMsg) {
   singleThreadSd->debugMsg = debugMsg;
   sendMsg(singleThreadSd, msg);
}                     
#endif


// broadcast a message to all media control clients
void broadcastMsg(const char * msg) {

   LinkedList * l;
   struct socketDescriptor * sd;
   int clientsConnected=FALSE;
   
   int msgLen = strlen(msg);
   char * crMsg = (char *) malloc(msgLen + 2);
   strcpy(crMsg, msg);
   if (crMsg[msgLen-1]!='\r') 
      strcat(crMsg, "\r");
   
   if (!serverList)
      serverList = new LinkedList();   

   for (l=serverList->first(); l->isNotLast(); l=l->next()) {
      sd = (struct socketDescriptor *) l->get();
      
      if (!sd->connected) {
         // can't send updates to disconnected clients
         continue;
      }
      
      clientsConnected=TRUE;   
         
      if (send(sd->socket, crMsg, strlen(crMsg), 0)==SOCKET_ERROR &&
          WSAGetLastError()!=WSAECONNABORTED) {
         // an aborted connection error is normal on a half closed connection
         printErrorMsg("broadcastMsg");
      }
   }
   
   if (clientsConnected)
      printf("Broadcasting msg: %s\n", msg);
      
   free(crMsg);   
}


// receive and process client commands
DWORD rxServerThread(LPVOID arg) {

   struct socketDescriptor * sd = (struct socketDescriptor *) arg;
   char buffer[1024], msg[2048], * delim;
   int nBytes;
   LinkedList * l;
   
   SOCKET socket = sd->socket;
   
   msg[0]='\0';

   while(1) {
      nBytes=recv(socket, buffer, sizeof(buffer-1), 0);
      buffer[nBytes]='\0';
      
      if (nBytes==0 || nBytes==SOCKET_ERROR || sd->exit) {
         
         if (!sd->exit) {
            if (nBytes==SOCKET_ERROR) {
               printErrorMsg("rxServerThread");
               closesocket(socket);
               sd->socket = INVALID_SOCKET;
            } else {      
               // close the connection normally
               processClientDisconnect(sd);
               shutdownConnection(sd);
            } 
         }   
         
         sd->connected = FALSE;
         free((char *) sd->host);
         free((char *) sd->description);
         CloseHandle(sd->hThread);
         sd->hThread = NULL;
         printq("rxServerThread exit socket %d\n", socket);
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
            
            processClientMsg(msg, sd);
            
            strcpy(msg, delim+1);
         }
      }
   }
}


// receive and process agent/server notifications
DWORD rxClientThread(LPVOID arg) {  
   char buffer[1024], msg[2048], * delim;
   int nBytes;
   struct socketDescriptor * sd = (struct socketDescriptor *) arg;
   SOCKET socket=sd->socket;
   msg[0]='\0';
   
   addSocketDescriptorToClientList(sd);
   
   while(!sd->exit) {
      if (!sd->connected) {
         if (establishConnection(sd))
            socket=sd->socket;
         else 
            Sleep(10000);
      } else {  
      
         nBytes=recv(sd->socket, buffer, sizeof(buffer-1), 0);
         
         if (nBytes==0) {
            // graceful close
            shutdownConnection(sd);
         }   
         else if(nBytes==SOCKET_ERROR) {
            // forceful close
            if (WSAGetLastError()!=WSAECONNABORTED) {
               // an aborted connection error is normal on a half closed connection
               printErrorMsg("rxClientThread");
            }   
            shutdownConnection(sd);
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
               processServerMsg(msg, sd);
               strcpy(msg, delim+1);
            }   
         }
      }
   }
   
   CloseHandle(sd->hThread);
   sd->hThread=NULL;
   printq("rxClientThread exit host %s socket %d\n", sd->host, socket);
   ExitThread(0);
   
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


// return a string formatted with the current time as HH.MM.SS.MMM
char * szTime() {
   SYSTEMTIME tm;
   static char szTm[13];
   GetLocalTime(&tm);
   snprintf(szTm, sizeof(szTm), "%02d.%02d.%02d.%03d", 
            tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
   return szTm;
}


void addSocketDescriptorToClientList(socketDescriptor * sd) {

   LinkedList * l;

   if (!clientList)
      clientList = new LinkedList();
      
   for (l=clientList->first(); l->isNotLast(); l=l->next()) {
      if (sd==(struct socketDescriptor *) l->get())
         break;   // socket descriptor is already in the list
   }
   if (l->isLast()) {
      l->add(sd);
   }
}      


BOOL areSocketThreadsRunning() {

   LinkedList * l;
   socketDescriptor * sd;
   
   if (serverList) {
      for (l=serverList->first(); l->isNotLast(); l=l->next()) {
         sd = (struct socketDescriptor *) l->get();
         if (sd->hThread)
            return TRUE;
      }
   }   
   
   if (clientList) {
      for (l=clientList->first(); l->isNotLast(); l=l->next()) {
         sd = (struct socketDescriptor *) l->get();
         if (sd->hThread)
            return TRUE;
      }
   }   
   
   return FALSE;
} 


void printErrorMsg(const char * prefix) {
   char * msgBuf;
   const char * format=NULL;
   DWORD dw = GetLastError(); 

   if (dw==WSAECONNRESET)
      format = "%s: remote host forcibly closed the connection\n";
   else if (dw==WSAECONNREFUSED)
      format = "%s: server refused connection request\n";
   else {   
      FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | 
         FORMAT_MESSAGE_FROM_SYSTEM |
         FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL,
         dw,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         (LPSTR)&msgBuf,
         0, NULL );
   }     
   
   if (format)
      printq(format, prefix);
   else {
      printq("\n%s error(%d): %s\n", prefix, dw, msgBuf);
      LocalFree(msgBuf);   // allocated by FORMAT_MESSAGE_ALLOCATE_BUFFER
   }      
}


// queue log messages when multithreading i.e. when isClientMsgThreadRunning
// dequeue and print from a single thread to prevent jumbling text
// from concurrent print statements
// 
// vprintf directly when running a single thread
int printq(const char * format, ...) {
   int nChars;
   va_list args;
   va_start(args, format);

#ifdef MCS   
   if (isClientMsgThreadRunning) {
      char sz[2048];
      nChars=vsnprintf(sz, sizeof(sz), format, args);
      printQueueAdd(sz);
   } else {   
      nChars=vprintf(format, args);
   }   
#else    
   nChars=vprintf(format, args);
#endif       

   va_end(args);
   return nChars;
}   


void printErrorAndExit(const char * prefix) {
   DWORD dw = GetLastError(); 
   printConsoleLogMsgs();   
   printErrorMsg(prefix);
   ExitProcess(dw); 
}


void printConsoleLogMsgs() {
#ifdef MCS
   char * logMsg;
   // print console log messages from a single thread to avoid jumbling
   // text from concurrent print statements
   while(logMsg = printQueueRemove()) {
      printf("%s", logMsg);
      free(logMsg);
   }   
#endif   
}   


#ifndef MCS
// stub out hooks to functions unused by demo utilties
void processClientDisconnect(struct socketDescriptor * sd) {}
void processClientMsg(const char * msg, struct socketDescriptor * sd) {
   printf("processClientMsg from host %s: %s\n", sd->host, msg);
}
void processServerConnect(const char * agent) {}
void processServerMsg(const char * msg, struct socketDescriptor * sd) {
   printf("processServerMsg from host %s: %s\n", sd->host, msg);
}
#endif  // #ifndef MCS




