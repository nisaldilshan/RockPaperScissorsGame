#pragma once
#include "NetworkMessage.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")


class Server
{
public:
    Server(/* args */);
    ~Server();
    static Server& get();
    void waitForConnection();
    void sendData(const char * buffer, int length);
    int recieveData(char * buffer, int length);
private:
    SOCKET m_serverSocket = INVALID_SOCKET;
    SOCKET m_clientSocket = INVALID_SOCKET;
};




