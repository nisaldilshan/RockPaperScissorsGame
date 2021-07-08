#include "Player.h"
#include <random>

ComputerPlayer::ComputerPlayer(/* args */)
{
}

ComputerPlayer::~ComputerPlayer()
{
}

Gesture ComputerPlayer::getInput() 
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,3); // distribution in range [1, 6]

    //std::cout << dist(rng) << std::endl;

    // switch(dist(rng)) {
    // case 1:
    //     return Gesture::Rock;
    // case 2:
    //     return Gesture::Paper;
    // case 3:
    //     return Gesture::Scissor;
    // default:
    //     return Gesture::None;
    // }

    return static_cast<Gesture>(dist(rng));
    
}

bool ComputerPlayer::getPlayAgainInput() 
{
    return true;
}
