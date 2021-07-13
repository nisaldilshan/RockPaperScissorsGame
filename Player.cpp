#include "Player.h"

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



