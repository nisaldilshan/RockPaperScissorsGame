#include "Player.h"

Player::Player(/* args */)
{
}

Player::~Player()
{
}

Gesture Player::play() 
{
    Gesture in = getInput();
    m_playAgain = getPlayAgainInput();
    return in;
}

bool Player::wantToPlayAgain() 
{
    return m_playAgain;
}

void Player::processInput(Gesture input) 
{
    
}



