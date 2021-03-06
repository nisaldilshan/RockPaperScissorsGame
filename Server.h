#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")


class Server
{
public:
    Server(/* args */);
    ~Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;
    static Server& get();
    void start(const char* port);
    void waitForConnection();
    void sendData(const char * buffer, int length);
    int recieveData(char * buffer, int length);
private:
    SOCKET m_serverSocket = INVALID_SOCKET;
    SOCKET m_clientSocket = INVALID_SOCKET;
};




