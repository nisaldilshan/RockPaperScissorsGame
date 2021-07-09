#pragma once

enum class Gesture { 
    Rock, 
    Paper, 
    Scissor,
    None
}; 

class Player
{
public:
    Player(/* args */);
    ~Player();
    Gesture play();
    bool wantToPlayAgain();

private:
    bool m_playAgain = true;
    virtual Gesture getInput() = 0;
    virtual bool getPlayAgainInput() = 0;
    //void validateInput();
    void processInput(Gesture input);
};


class ComputerPlayer :public Player
{
public:
    ComputerPlayer(/* args */);
    ~ComputerPlayer();
private:
    virtual Gesture getInput();
    virtual bool getPlayAgainInput();
};




class HumanPlayer : public Player
{
public:
    HumanPlayer(/* args */);
    ~HumanPlayer();
private:
    virtual Gesture getInput();
    virtual bool getPlayAgainInput();
};





