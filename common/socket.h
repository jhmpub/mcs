#include <winsock2.h>

#ifndef SOCKET_H
#define SOCKET_H

#define KBUFFERSIZE 1024

struct socketDescriptor {
   const char * host;
   int port;
   SOCKET * socket;
   int registered;             // optional
   int exit;                   // internal
   const char * description;   // optional
   const char * reconnectMsg;  // optional, establishConnection sends if non-null 
   const char * debugMsg;      // optional, console log msg when reconnectMsg sent
};   

char * getHostname(void);
char * initWinsock(void);
char * lookupName(const char *, char *, int);
char * szTime(void);
DWORD rxClientThread(LPVOID param);
DWORD rxServerThread(LPVOID param);
DWORD socketListen(LPVOID);
int isLocked(const char *);
int establishConnection(struct socketDescriptor *);
SOCKET socketBind(const char * pcAddress, int nPort);
u_long lookupAddress(const char *);
void broadcastMsg(const char *);
void clrReconnectMsg(struct socketDescriptor *);
void initTcpServer(const char *, int);
void printErrorAndExit(const char *);
void printErrorMsg(const char *);
void processClientMsg(struct socketDescriptor *, char *);
void processServerConnect(const char *);
void processServerDisconnect(int);
void processAgentNotification(const char *, char *);
void rxClientThreadExit(struct socketDescriptor *);
void sendMsg(const char *);
void sendMsg(struct socketDescriptor *, const char *);
void sendMsg(struct socketDescriptor *, const char *, const char *);
void setReconnectMsg(struct socketDescriptor *, const char *, const char *);
void shutdownConnection(struct socketDescriptor *);
void shutdownServer(void);
void unlock(void);

#endif // ifndef SOCKET_H



