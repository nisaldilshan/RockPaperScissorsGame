#include "Player.h"
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
    return Gesture::None;
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