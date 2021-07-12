#include "Player.h"
#include "Server.h"
#include "Client.h"
#include "Util.h"
#include <iostream>
#include <cassert>
#include <sstream>

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
    int bytes = server.recieveData(recvbuf, DEFAULT_BUFLEN);
    
    NetworkMessage msg;
    memcpy(&msg, recvbuf, sizeof(NetworkMessage));

    Gesture ges = Gesture::None;
    if (msg.type == MessageType::PlayerInput)
        memcpy(&ges, &msg.data, sizeof(Gesture));
    else
        Util::Error("Expected PlayerInput message");


    // send Ack
    NetworkMessage sendAckMsg;
    sendAckMsg.type = MessageType::Ack;
    server.sendData((const char*)&sendAckMsg, sizeof(NetworkMessage));

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
    char recvbuf[DEFAULT_BUFLEN];
    Server& server = Server::get();
    int bytes = server.recieveData(recvbuf, DEFAULT_BUFLEN);

    NetworkMessage msg;
    memcpy(&msg, recvbuf, sizeof(NetworkMessage));

    bool playAgain = true;
    if (msg.type == MessageType::PlayerPlayAgainInput)
        memcpy(&playAgain, &msg.data, sizeof(bool));
    else
        Util::Error("Expected PlayerPlayAgainInput message");

    // send Ack
    NetworkMessage sendAckMsg;
    sendAckMsg.type = MessageType::Ack;
    server.sendData((const char*)&sendAckMsg, sizeof(NetworkMessage));

    if (playAgain)
        std::cout << "recieved playAgain::Yes "  << std::endl;
    else 
        std::cout << "recieved playAgain::No "  << std::endl;

    return playAgain;
}

void announceWinnerLocal(RoundResult winner) 
{
    std::cout << (winner== RoundResult::Draw ? "Draw" : winner == RoundResult::PlayerOneWins ? "PlayerOneWins" : "PlayerTwoWins") << std::endl;
}

void announceSummaryLocal(std::string summary) 
{
    std::cout << summary << std::endl;
}

void announceWinnerNetwork(RoundResult winner) 
{
    Server& server = Server::get();
    NetworkMessage winnerMsg;
    winnerMsg.type = MessageType::Winner;
    memcpy(winnerMsg.data, &winner, sizeof(RoundResult));
    server.sendData((const char*)&winnerMsg, sizeof(NetworkMessage));

    // wait for ack
    char recvbuf[DEFAULT_BUFLEN];
    int bytes = server.recieveData(recvbuf, DEFAULT_BUFLEN);
    NetworkMessage ackMsg;
    memcpy(&ackMsg, recvbuf, sizeof(NetworkMessage));

    if (ackMsg.type == MessageType::Ack)
        std::cout << "Acknowledged" <<std::endl;
    else
        Util::Error("Expected Ack message");
}

void announceSummaryNetwork(std::string summary) 
{
    Server& server = Server::get();
    NetworkMessage summaryMsg;
    summaryMsg.type = MessageType::GameSummary;
    assert(summary.length() <= SIZEOF_DATA);
    summaryMsg.length = (int)summary.length();
    const char * s = summary.c_str();
    memcpy(summaryMsg.data, s, summary.length());
    server.sendData((const char*)&summaryMsg, sizeof(NetworkMessage));

    // wait for ack
}


HumanPlayer::HumanPlayer(bool isLocal, bool isHost)
{
    if (isLocal)
    {
        m_getInputFunc = std::bind(getLocalInput);
        m_getPlayAgainInputFunc = std::bind(getLocalPlayAgainInput);
        m_announceWinnerFunc = std::bind(announceWinnerLocal, std::placeholders::_1);
        m_announceSummaryFunc = std::bind(announceSummaryLocal, std::placeholders::_1);

        if (!isHost)
        {
            Client& client = Client::get();
            client.connectToServer();
        }
    }
    else
    {
        m_getInputFunc = std::bind(getNetworkInput);
        m_getPlayAgainInputFunc = std::bind(getNetworkPlayAgainInput);
        m_announceWinnerFunc = std::bind(announceWinnerNetwork, std::placeholders::_1);
        m_announceSummaryFunc = std::bind(announceSummaryNetwork, std::placeholders::_1);

        Server& server = Server::get();
        server.waitForConnection();
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

void HumanPlayer::announceWinner(RoundResult winner) 
{
    m_announceWinnerFunc(winner);
}

void HumanPlayer::announceSummary(std::string summary) 
{
    m_announceSummaryFunc(summary);
}
