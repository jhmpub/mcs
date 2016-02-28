// This is a simple utility that transmits UDP messages
// to the acServer.exe daemon.
//
// Usage: msg.exe ServerName msg 
//
// compiles with cygwin gcc - http://cygwin.com
// gcc -mno-cygwin -mwindows -o msg msg.c -l wsock32
//
// 2004 Aug 8 jhm original creation
//

#include <winsock.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include "../../common/tira.h"
#include "../../common/xantech_codes.h"
#include "../../common/yamaha_codes.h"
#include "../../common/common.h"

char buf[1024];

// Function prototype
void DatagramClient(char *, short, char *);

// Helper macro for displaying errors
#define PRINTERROR(s)	\
		printf("\n%: %d\n", s, WSAGetLastError());

////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;

	//
	// Check for the host and msg arguments
	//
	if (argc != 3)
	{
		printf("\nUsage: msg.exe ServerName msg\n");
		return 1;
	}

	//
	// Initialize WinSock and check the version
	//
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		printf("\nError: wrong winsock version\n");
		return 1;
	}

	//
	// Go do all the stuff a datagram client does
	//
	DatagramClient(argv[1], AUDIO_CONTROL_REQUEST_PORT, argv[2]);
	
	//
	// Release WinSock
	//
	WSACleanup();
}

////////////////////////////////////////////////////////////

void DatagramClient(char *szServer, short nPort, char * msg)
{

	printf("\nDatagram Client sending message \"%s\" to server %s on port %d\n",
			msg, szServer, nPort);

	//
	// Find the server
	//
    LPHOSTENT lpHostEntry;
	lpHostEntry = gethostbyname(szServer);
    if (lpHostEntry == NULL)
    {
        PRINTERROR("gethostbyname()");
        return;
    }


	//
	// Create a UDP/IP datagram socket
	//
	SOCKET	theSocket;

	theSocket = socket(AF_INET,			// Address family
					   SOCK_DGRAM,		// Socket type
					   IPPROTO_UDP);	// Protocol
	if (theSocket == INVALID_SOCKET)
	{
		PRINTERROR("socket()");
		return;
	}

	//
	// Fill in the address structure for the server
	//
	SOCKADDR_IN saServer;

	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
										// ^ Server's address
	saServer.sin_port = htons(nPort);	// Port number from command line

	//
	// Send data to the server
	//
	int nRet;

	nRet = sendto(theSocket,				// Socket
				  msg,                      // Data buffer
				  strlen(msg),              // Length of data
				  0,						// Flags
				  (LPSOCKADDR)&saServer,	// Server address
				  sizeof(struct sockaddr)); // Length of address
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("sendto()");
		closesocket(theSocket);
		return;
	}

	//
	// Wait for the reply
	//
	//memset(szBuf, 0, sizeof(szBuf));
	//int nFromLen;

	//nFromLen = sizeof(struct sockaddr);
	//recvfrom(theSocket,						// Socket
	//		 szBuf,							// Receive buffer
	//		 sizeof(szBuf),					// Length of receive buffer
	//		 0,								// Flags
	//		 (LPSOCKADDR)&saServer,			// Buffer to receive sender's address
	//		 &nFromLen);					// Length of address buffer
	//if (nRet == SOCKET_ERROR)
	//{
	//	PRINTERROR("recvfrom()");
	//	closesocket(theSocket);
	//	return;
	//}

	//
	// Display the data that was received
	//
   	//sprintf(buf, "\nData received: %s", szBuf);
    //cout << buf;
    
	closesocket(theSocket);
	return;
}
