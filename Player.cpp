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
    return getPlayAgainInput();
}

void Player::processInput(Gesture input) 
{
    
}



