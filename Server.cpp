#include "Server.h"

Server& Server::get() {
  static Server instance;
  return instance;
}

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
        std::cout << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        //return 1;
    }

    
    // Create a SOCKET for the server to listen for client connections
    m_serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_serverSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        //return 1;
    }

    /////Binding socket
    // Setup the TCP listening socket
    iResult = bind( m_serverSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(m_serverSocket);
        WSACleanup();
        //return 1;
    }
    freeaddrinfo(result);

}

Server::~Server()
{
    // shutdown the connection since we're done
    int iResult = shutdown(m_clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        closesocket(m_clientSocket);
        WSACleanup();
        //return 1;
    }

    // cleanup
    closesocket(m_clientSocket);
    WSACleanup();
}

void Server::waitForConnection() 
{
    ////listening 
    if ( listen( m_serverSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        std::cout <<  "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(m_serverSocket);
        WSACleanup();
        //return 1;
    }

    // Accept a client socket
    m_clientSocket = accept(m_serverSocket, NULL, NULL);
    if (m_clientSocket == INVALID_SOCKET) {
        std::cout << "accept failed: " << WSAGetLastError() << std::endl;
        closesocket(m_serverSocket);
        WSACleanup();
        //return 1;
    }

    std::cout << "Success : client connected to server" << std::endl;
}

int Server::recieveData(char * buffer, int length) 
{
    int recvLength = 0;
    while (recvLength < DEFAULT_BUFLEN)
    {
        int iResult = recv(m_clientSocket, buffer, length, 0);
        if (iResult > 0) {
            std::cout << "Bytes received: " << iResult << std::endl;
            recvLength += iResult;
        } 
        else if (iResult == 0)
            std::cout << "Connection closing..." << std::endl;
        else {
            std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            closesocket(m_clientSocket);
            WSACleanup();
            //return 1;
        }
    }
    
    return recvLength;
}

void Server::sendData(const char * buffer, int length) 
{
    // Echo the buffer back to the sender
    int iSendResult = send(m_clientSocket, buffer, length, 0);
    if (iSendResult == SOCKET_ERROR) {
        std::cout << "send failed: " << WSAGetLastError() << std::endl;
        closesocket(m_clientSocket);
        WSACleanup();
        //return 1;
    }
    std::cout << "Bytes sent: " << iSendResult << std::endl;
}
