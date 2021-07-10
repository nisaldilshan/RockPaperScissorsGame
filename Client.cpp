#include "Client.h"

Client::Client(/* args */)
{
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        //return 1;
    }


    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    constexpr char* DEFAULT_PORT = "27015";
    constexpr char* DEFAULT_IP = "127.0.0.1";

    // Resolve the server address and port
    iResult = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        //return 1;
    }

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr=result;

    // Create a SOCKET for connecting to server
    m_Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (m_Socket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
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
        printf("Unable to connect to server!\n");
        WSACleanup();
        //return 1;
    }

}

Client::~Client()
{
    // shutdown the send half of the connection since no more data will be sent
    int iResult = shutdown(m_Socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(m_Socket);
        WSACleanup();
        //return 1;
    }

    // cleanup
    closesocket(m_Socket);
    WSACleanup();

}

void Client::sendData(char * buffer, int length) 
{
    int iResult = send(m_Socket, buffer, length, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(m_Socket);
        WSACleanup();
        //return 1;
    }
    printf("Bytes Sent: %ld\n", iResult);
}

void Client::recieveData(char * buffer, int length) 
{
    // Receive data until the server closes the connection
    int iResult;
    do {
        iResult = recv(m_Socket, buffer, length, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);
}
