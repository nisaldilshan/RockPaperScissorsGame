#include "Player.h"
#include <iostream>

HumanPlayer::HumanPlayer(/* args */)
{
}

HumanPlayer::~HumanPlayer()
{
}

Gesture HumanPlayer::getInput() 
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

bool HumanPlayer::getPlayAgainInput() 
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