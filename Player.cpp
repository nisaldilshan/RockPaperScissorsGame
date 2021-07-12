#include "Player.h"

Player::Player(/* args */)
{
}

Player::~Player()
{
}

Gesture Player::play() 
{
    return getInput();
}

bool Player::wantToPlayAgain() 
{
    m_playAgain = getPlayAgainInput();
    return m_playAgain;
}

void Player::processInput(Gesture input) 
{
    
}



