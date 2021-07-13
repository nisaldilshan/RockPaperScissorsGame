#include "Player.h"
#include <random>

ComputerPlayer::ComputerPlayer(/* args */)
{
}

Gesture ComputerPlayer::getInput() 
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,3); // distribution in range [1, 6]

    return static_cast<Gesture>(dist(rng));
}

bool ComputerPlayer::getPlayAgainInput() 
{
    return true;
}

void ComputerPlayer::announceWinner(RoundResult winner, bool playAgain) 
{
    
}

void ComputerPlayer::announceSummary(std::string summary) 
{
    
}
