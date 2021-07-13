#include "Server.h"
#include "Util.h"

Server& Server::get() {
  static Server instance;
  return instance;
}

Server::Server()
{
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        Util::Error("WSAStartup failed: " + std::to_string(iResult));
    }
}

Server::~Server()
{
    // shutdown the connection since we're done
    int iResult = shutdown(m_clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        closesocket(m_clientSocket);
        WSACleanup();
        Util::Error("shutdown failed with error: " + std::to_string(WSAGetLastError()));
    }

    // cleanup
    closesocket(m_clientSocket);
    WSACleanup();
}

void Server::start(const char* port) 
{
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    int iResult = getaddrinfo(NULL, port, &hints, &result);
    if (iResult != 0) {
        WSACleanup();
        Util::Error("getaddrinfo failed: " + std::to_string(iResult));
    }

    
    // Create a SOCKET for the server to listen for client connections
    m_serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_serverSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        Util::Error("Error at socket(): " + std::to_string(WSAGetLastError()));
    }

    /////Binding socket
    // Setup the TCP listening socket
    iResult = bind( m_serverSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(m_serverSocket);
        WSACleanup();
        Util::Error("bind failed with error: " + std::to_string(WSAGetLastError()));
    }
    freeaddrinfo(result);
}

void Server::waitForConnection() 
{
    ////listening 
    if ( listen( m_serverSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        closesocket(m_serverSocket);
        WSACleanup();
        Util::Error("Listen failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Accept a client socket
    m_clientSocket = accept(m_serverSocket, NULL, NULL);
    if (m_clientSocket == INVALID_SOCKET) {
        closesocket(m_serverSocket);
        WSACleanup();
        Util::Error("accept failed: " + std::to_string(WSAGetLastError()));
    }

    Util::Log("Success : client connected to server");
}

int Server::recieveData(char * buffer, int length) 
{
    int recvLength = 0;
    while (recvLength < length)
    {
        int iResult = recv(m_clientSocket, buffer, length - recvLength, 0);
        if (iResult > 0) {
            Util::Log("Bytes received: " + std::to_string(iResult));
            recvLength += iResult;
        } 
        else if (iResult == 0)
            Util::Error("Connection closing...");
        else {
            closesocket(m_clientSocket);
            WSACleanup();
            Util::Error("recv failed: " + std::to_string(WSAGetLastError()));
        }
    }
    
    return recvLength;
}

void Server::sendData(const char * buffer, int length) 
{
    // Echo the buffer back to the sender
    int iSendResult = send(m_clientSocket, buffer, length, 0);
    if (iSendResult == SOCKET_ERROR) {
        closesocket(m_clientSocket);
        WSACleanup();
        Util::Error("send failed: " + std::to_string(WSAGetLastError()));
    }

    Util::Log("Bytes sent: " + std::to_string(iSendResult));
}
