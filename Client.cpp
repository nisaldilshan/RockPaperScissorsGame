#include "Client.h"
#include "Util.h"

Client& Client::get() 
{
    static Client instance;
    return instance;
}

Client::Client(/* args */)
{
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        Util::Error("WSAStartup failed: " + std::to_string(iResult));
    }
}

Client::~Client()
{
    // shutdown client socket
    int iResult = shutdown(m_Socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        closesocket(m_Socket);
        WSACleanup();
        Util::Error("shutdown failed: " + std::to_string(WSAGetLastError()));
    }

    // cleanup
    closesocket(m_Socket);
    WSACleanup();
}

void Client::connectToServer(const char* ip, const char* port) 
{
    struct addrinfo *result = NULL, hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int iResult = getaddrinfo(ip, port, &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed: " <<  iResult << std::endl;
        WSACleanup();
        //return 1;
    }

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    struct addrinfo *ptr=result;

    // Create a SOCKET for connecting to server
    m_Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (m_Socket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " <<  WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        //return 1;
    }

    // Connect to server.
    iResult = connect( m_Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (m_Socket == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl;
        WSACleanup();
        //return 1;
    }

    Util::Log("Success : client connected to server");
}

void Client::sendData(const char * buffer, size_t length) 
{
    int iResult = send(m_Socket, buffer, (int)length, 0);
    if (iResult == SOCKET_ERROR) {
        closesocket(m_Socket);
        WSACleanup();
        Util::Error("send failed: " + std::to_string(WSAGetLastError()));
    }
    Util::Log("Bytes Sent: " + std::to_string(iResult));
}

int Client::recieveData(char * buffer, int length) 
{
    int recvLength = 0;
    while (recvLength < length)
    {
        int iResult = recv(m_Socket, buffer, length - recvLength, 0);
        if (iResult > 0) {
            Util::Log("Bytes received: " + std::to_string(iResult));
            recvLength += iResult;
        } 
        else if (iResult == 0)
            Util::Error("Connection closing...");
        else {
            closesocket(m_Socket);
            WSACleanup();
            Util::Error("recv failed: " + std::to_string(WSAGetLastError()));
        }
    }
    
    return recvLength;
}
