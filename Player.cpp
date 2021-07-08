#include "Player.h"

Player::Player(/* args */)
{
}

Player::~Player()
{
}

void Player::play() 
{
    getInput();
    m_playAgain = getPlayAgainInput();
}

bool Player::wantToPlayAgain() 
{
    return m_playAgain;
}



