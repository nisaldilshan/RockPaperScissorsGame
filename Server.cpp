#include "Server.h"

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

int Server::getData(char * buffer, int length) 
{
    int iResult = recv(m_clientSocket, buffer, length, 0);
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult);
    } else if (iResult == 0)
        printf("Connection closing...\n");
    else {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(m_clientSocket);
        WSACleanup();
        //return 1;
    }
    return iResult;
}

void Server::sendData(char * buffer, int length) 
{
    // Echo the buffer back to the sender
    int iSendResult = send(m_clientSocket, buffer, length, 0);
    if (iSendResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(m_clientSocket);
        WSACleanup();
        //return 1;
    }
    printf("Bytes sent: %d\n", iSendResult);
}
