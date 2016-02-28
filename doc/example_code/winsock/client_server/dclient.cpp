//
// DClient.cpp
//
// Extremely simple, totally useless datagram client example.
// Works in conjunction with DServer.cpp.
//
// The program attempts to connect to the server and port
// specified on the command line. The DServer program prints
// the needed information when it is started. Once connected,
// the program sends data to the server, waits for a response
// and then exits.
//
// Compile and link with wsock32.lib
//
// Pass the server name and port number on the command line. 
//
// Example: DClient MyMachineName 2000
//

#include <winsock.h>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string.h>


using namespace std;

char buf[1024];

// Function prototype
void DatagramClient(char *szServer, short nPort);

// Helper macro for displaying errors
#define PRINTERROR(s)	\
		sprintf(buf,"\n%: %d\n", s, WSAGetLastError()); cout << buf;

////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;
	short nPort;

	//
	// Check for the host and port arguments
	//
	if (argc != 3)
	{
		sprintf(buf,"\nSyntax: dclient ServerName PortNumber\n");
        cout << buf;
		return 1;
	}

	nPort = atoi(argv[2]);

	//
	// Initialize WinSock and check the version
	//
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		sprintf(buf,"\n Wrong version\n");
		return 1;
	}

	//
	// Go do all the stuff a datagram client does
	//
	DatagramClient(argv[1], nPort);
	
	//
	// Release WinSock
	//
	WSACleanup();
}

////////////////////////////////////////////////////////////

void DatagramClient(char *szServer, short nPort)
{

	sprintf(buf, "\nDatagram Client sending to server: %s on port: %d",
				szServer, nPort);
    cout << buf;            

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
	char szBuf[256];
	int nRet;

	strcpy(szBuf, "client msg\n");
	nRet = sendto(theSocket,				// Socket
				  szBuf,					// Data buffer
				  strlen(szBuf),			// Length of data
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
	memset(szBuf, 0, sizeof(szBuf));
	int nFromLen;

	nFromLen = sizeof(struct sockaddr);
	recvfrom(theSocket,						// Socket
			 szBuf,							// Receive buffer
			 sizeof(szBuf),					// Length of receive buffer
			 0,								// Flags
			 (LPSOCKADDR)&saServer,			// Buffer to receive sender's address
			 &nFromLen);					// Length of address buffer
	if (nRet == SOCKET_ERROR)
	{
		PRINTERROR("recvfrom()");
		closesocket(theSocket);
		return;
	}

	//
	// Display the data that was received
	//
   	sprintf(buf, "\nData received: %s", szBuf);
    cout << buf;
    
	closesocket(theSocket);
	return;
}
