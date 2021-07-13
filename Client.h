#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
    Client();
    ~Client();
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client(Client&&) = delete;
    Client& operator=(Client&&) = delete;
    static Client& get();
    void connectToServer(const char* ip, const char* port);
    void sendData(const char * buffer, size_t length);
    int recieveData(char * buffer, int length);
private:
    SOCKET m_Socket = INVALID_SOCKET;
};

    

