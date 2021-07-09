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
private:
    SOCKET m_clientSocket = INVALID_SOCKET;
};

Server::Server(/* args */)
{
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        //return 1;
    }

    //creating a socket

    constexpr char* DEFAULT_PORT = "27015";

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        //return 1;
    }

    SOCKET ListenSocket = INVALID_SOCKET;
    // Create a SOCKET for the server to listen for client connections
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        //return 1;
    }

    /////Binding socket
    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        //return 1;
    }
    freeaddrinfo(result);

    ////listening 
    if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        //return 1;
    }

    // Accept a client socket
    m_clientSocket = accept(ListenSocket, NULL, NULL);
    if (m_clientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        //return 1;
    }



    constexpr int DEFAULT_BUFLEN = 512;

    char recvbuf[DEFAULT_BUFLEN];
    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(m_clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = send(m_clientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(m_clientSocket);
                WSACleanup();
                break;
                //return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        } else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(m_clientSocket);
            WSACleanup();
            break;
            //return 1;
        }

    } while (iResult > 0);

}

Server::~Server()
{
    // shutdown the connection since we're done
    int iResult = shutdown(m_clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_clientSocket);
        WSACleanup();
        //return 1;
    }

    // cleanup
    closesocket(m_clientSocket);
    WSACleanup();
}
