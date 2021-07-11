#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
    Client(/* args */);
    ~Client();
    void sendData(const char * buffer, size_t length);
    void recieveData(char * buffer, int length);
private:
    SOCKET m_Socket = INVALID_SOCKET;
};



// shutdown the connection for sending since no more data will be sent
// the client can still use the ConnectSocket for receiving data

// iResult = shutdown(m_Socket, SD_SEND);
// if (iResult == SOCKET_ERROR) {
//     printf("shutdown failed: %d\n", WSAGetLastError());
//     closesocket(m_Socket);
//     WSACleanup();
//     //return 1;
// }

    

