#include "Player.h"
#include "Server.h"
#include "NetworkMessage.h"
#include <iostream>

Gesture getLocalInput()
{
    std::cout << "Enter your choice, \n1.Rock, 2.Paper, 3.Scissors : ";
    int inputGesture;
    std::cin >> inputGesture;
    while (inputGesture < 1 || inputGesture > 3)
    {
        std::cout << "Not a valid choice, Enter your choice again : ";
        std::cin >> inputGesture;
    }
    
    return static_cast<Gesture>(inputGesture);
}

bool getLocalPlayAgainInput()
{
    std::cout << "Do you want to play another round ? (Y/N) : ";
    char in;
    std::cin >> in;
    while (in != 'Y' && in != 'N' && in != 'y' && in != 'n')
    {
        std::cout << "Do you want to play another round ? (Y/N) : ";
        std::cin >> in;
    }

    return in == 'Y' || in == 'y';
}

Gesture getNetworkInput()
{
    char recvbuf[DEFAULT_BUFLEN];
    Server& server = Server::get();
    int bytes = server.getData(recvbuf, DEFAULT_BUFLEN);
    
    NetworkMessage msg;
    memcpy(&msg, recvbuf, sizeof(NetworkMessage));

    Gesture ges = Gesture::None;
    if (msg.type == MessageType::PlayerInput)
        memcpy(&ges, &msg.data, sizeof(Gesture));

    if (ges == Gesture::Rock)
        std::cout << "recieved Gesture::Rock "  << std::endl;
    else if (ges == Gesture::Paper)
        std::cout << "recieved Gesture::Paper "  << std::endl;
    else if (ges == Gesture::Scissor)
        std::cout << "recieved Gesture::Scissor "  << std::endl;
    else 
        std::cout << "recieved Gesture::None "  << std::endl;
    return ges;
}

bool getNetworkPlayAgainInput()
{
    return true;
}

HumanPlayer::HumanPlayer(bool isLocal)
{
    if (isLocal)
    {
        m_getInputFunc = std::bind(getLocalInput);
        m_getPlayAgainInputFunc = std::bind(getLocalPlayAgainInput);
    }
    else
    {
        Server& server = Server::get();
        server.waitForConnection();
        m_getInputFunc = std::bind(getNetworkInput);
        m_getPlayAgainInputFunc = std::bind(getNetworkPlayAgainInput);
    }
}

HumanPlayer::~HumanPlayer()
{
}

Gesture HumanPlayer::getInput() 
{
    return m_getInputFunc();
}

bool HumanPlayer::getPlayAgainInput() 
{
    return m_getPlayAgainInputFunc();
}