#ifndef SOCKET_H
#define SOCKET_H

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <windef.h>
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "common.h"

#define KBUFFERSIZE 1024

struct socketDescriptor {
   char * host;
   int port;
   SOCKET socket;
   HANDLE hThread;
   int lastError;              // minimize duplicate log messages
   int connected;
   int registered;             // optional
   int exit;                   // internal
   const char * description;   // optional
};   


#define DEFAULT_SOCKET_DESCRIPTOR { \
   (char *) "",                     \
   0,                               \
   INVALID_SOCKET,                  \
   NULL,                            \
   NO_ERROR,                        \
   FALSE,                           \
   FALSE,                           \
   FALSE,                           \
   "undefined",                     \
}   

BOOL areSocketThreadsRunning(void);
char * getHostname(void);
char * initWinsock(void);
char * lookupName(const char *, char *, int);
char * szTime(void);
DWORD rxClientThread(LPVOID param);
DWORD rxServerThread(LPVOID param);
DWORD serverListenThread(LPVOID);
HANDLE createServerListenThread(SOCKET);
int establishConnection(struct socketDescriptor *);
SOCKET socketBind(const char * pcAddress, int nPort);
socketDescriptor * allocateServerSocketDescriptor(socketDescriptor *);
u_long lookupAddress(const char *);
void addSocketDescriptorToClientList(socketDescriptor *);
void broadcastMsg(const char *);
void clrReconnectMsg(struct socketDescriptor *);
void copySocketDescriptor(struct socketDescriptor *, struct socketDescriptor *);
void initClientThread(struct socketDescriptor *);
void initTcpServer(char *, int);
void printConsoleLogMsgs(void);
void printErrorAndExit(const char *);
void printErrorMsg(const char *);
void processClientDisconnect(struct socketDescriptor *);
void processClientMsg(const char *, struct socketDescriptor *);
void processServerConnect(const char *);
void processServerDisconnect(struct socketDescriport *);
void processServerMsg(const char *, struct socketDescriptor *);
void rxClientThreadExit(struct socketDescriptor *);
void sendMsg(const char *);
void sendMsg(const char *, const char *);
void sendMsg(struct socketDescriptor *, const char *);
void sendMsg(struct socketDescriptor *, const char *, const char *);
void setReconnectMsg(struct socketDescriptor *, const char *, const char *);
void shutdownConnection(struct socketDescriptor *);
void shutdownServer(void);

#endif // ifndef SOCKET_H



