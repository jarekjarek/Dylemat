// Dylematwieznia.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512 //domyslnty bufor
#define DEFAULT_PORT "27015" // domyslny port

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{


WSADATA wsaData;
int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket1 = INVALID_SOCKET; //dklaracja socketow
SOCKET ClientSocket2 = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result); // podaje port na jakim dziala
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

cout << "Wait for ClientSocket1 and ClientSocket2" << endl;

    // Accept a client socket 1
    ClientSocket1 = accept(ListenSocket, NULL, NULL);
    if (ClientSocket1 == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

// Accept a client socket 2
    ClientSocket2 = accept(ListenSocket, NULL, NULL);
    if (ClientSocket2 == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

////////////////////////////////////////////////////////////
int ile_rund,wybor,i; 
int lat_wiezien_1 = 0; //ilosc lat
int lat_wiezien_2 = 0; //ilosc lat
char wiezien1[DEFAULT_BUFLEN], wiezien2[DEFAULT_BUFLEN];
char *wygrana = "Dosta³eœ mniej lat od swojego kolegi";
char *przegrana = "Dosta³eœ wiecej lat od swojego kolegi";


cout << "Ile maja zagrac: ";
cin >> ile_rund;
cout << endl;

send (ClientSocket1, (char*)&ile_rund, sizeof(ile_rund), 0);
send (ClientSocket2, (char*)&ile_rund, sizeof(ile_rund), 0);



i=0;
while(i<ile_rund)
{
recv(ClientSocket1, wiezien1, 1, 0);
cout << "wiezien 1 - runda nr " << i+1 << " zagral " << wiezien1[0] << endl;
recv(ClientSocket2, wiezien2, 1, 0);
cout << "wiezien 2 - runda nr " << i+1 << " zagral " << wiezien2[0] << endl;

send( ClientSocket1, wiezien2, 1, 0 );
send( ClientSocket2, wiezien1, 1, 0 );

if ( wiezien1[0] == 'F' && wiezien2[0] == 'F')wybor=1;
if ( wiezien1[0] == 'U' && wiezien2[0] == 'U')wybor=2;
if ( wiezien1[0] == 'F' && wiezien2[0] == 'U')wybor=3;
if ( wiezien1[0] == 'U' && wiezien2[0] == 'F')wybor=4;


switch(wybor)
{
case 1:
lat_wiezien_1+=1;
lat_wiezien_2+=1;
cout << "Byli uczciwi dostali po 1 roku " << endl << endl;
break;
case 2:
lat_wiezien_1+=5;
lat_wiezien_2+=5;
cout << "Byli nie uczciwi dostali po 5 lat" << endl << endl;
break;
case 3:
lat_wiezien_1+=10;
lat_wiezien_2+=0;
cout << "Wiezien 1 byl uczciwy dostaje 10 lat" << endl;
cout << "Wiezien 2 byl nie uczciwy i nic nie dostaje" << endl << endl;
break;
case 4:
lat_wiezien_1+=0;
lat_wiezien_2+=10;
cout << "Wiezien 1 byl nie uczciwy i nic nie dostaje" << endl;
cout << "Wiezien 2 byl uczciwy dostaje 10 lat" << endl << endl;

}
i++;
}
cout << "wiezien 1 dosta³ lat: " << lat_wiezien_1 << endl;
cout << "wiezien 2 dosta³ lat: " << lat_wiezien_2 << endl;



if (lat_wiezien_1 > lat_wiezien_2)
{
send( ClientSocket1, przegrana, (int)strlen(przegrana), 0 );
send( ClientSocket2, wygrana, (int)strlen(wygrana), 0 );
}
if (lat_wiezien_1 < lat_wiezien_2)
{
send( ClientSocket1, wygrana, (int)strlen(wygrana), 0 );
send( ClientSocket2, przegrana, (int)strlen(przegrana), 0 );
}

/////////////////////////////////////////////////////////////////////

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket1, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket1);
        WSACleanup();
        return 1;
    }
iResult = shutdown(ClientSocket2, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket2);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket1);
closesocket(ClientSocket2);
    WSACleanup();

getch();

return 0;
}
