#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <time.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int _tmain(int argc, _TCHAR* argv[])
{



WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result); //podajes ip i port
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

ptr=result;

// Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
        ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
}

    // Connect to server.
    iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }


    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

//------------------------------------------------------------

int ile,wybor,los,los1,i;
char kolega[1];


recv(ConnectSocket, (char *)&ile, sizeof(ile), 0);

cout << "ile rund gramy: " << ile << endl;



srand(time(0));
kolega[0] ='0';

i=0;

while (i<ile)
{

los = rand()%2;
los1 = rand()%100+30;
if (kolega[0] =='U')los1-=30;
if (kolega[0] =='F')los1+=30;

if (los == 1)
{if (los1<65)wybor=1; else wybor=2;}
else
{if (los1<65)wybor=2; else wybor=1;}


switch(wybor)
{
case 1:
send( ConnectSocket, "U", 1, 0 );
cout << "Moje zagranie w rundzie nr " << i+1 << " jest Uczciwe " << endl;
break;
case 2:
send( ConnectSocket, "F", 1, 0 );
cout << "Moje zagranie w rundzie nr " << i+1 << " jest Nieuczciwe" << endl;
}


recv(ConnectSocket, recvbuf, recvbuflen, 0);
kolega[0] = recvbuf[0];

i++;
}





///////////////////////////////////////////////////////////////////////////////

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

getch();

return 0;
}



