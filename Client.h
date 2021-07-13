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
    static Client& get();
    void connectToServer();
    void sendData(const char * buffer, size_t length);
    int recieveData(char * buffer, int length);
private:
    SOCKET m_Socket = INVALID_SOCKET;
};

    

