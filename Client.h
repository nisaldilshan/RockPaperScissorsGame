#pragma once
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class Client
{
private:
    /* data */
public:
    Client(/* args */);
    ~Client();
};

Client::Client(/* args */)
{
    
    WSADATA wsaData;



    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        //return 1;
    }
}

Client::~Client()
{
}
