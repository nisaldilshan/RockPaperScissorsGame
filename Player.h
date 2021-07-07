class Player
{
public:
    Player(/* args */);
    ~Player();
    virtual void play() = 0;
    bool wantToPlayAgain();
protected:
    bool m_playAgain = true;
private:

};

Player::Player(/* args */)
{
}

Player::~Player()
{
}

bool Player::wantToPlayAgain() 
{
    return m_playAgain;
}

class ComputerPlayer :public Player
{
public:
    ComputerPlayer(/* args */);
    ~ComputerPlayer();
    virtual void play() override;
private:
    /* data */
};

ComputerPlayer::ComputerPlayer(/* args */)
{
}

ComputerPlayer::~ComputerPlayer()
{
}

void ComputerPlayer::play() 
{
    m_playAgain = true;
}


class HumanPlayer : public Player
{
public:
    HumanPlayer(/* args */);
    ~HumanPlayer();
    virtual void play() override;
private:
    /* data */
};

HumanPlayer::HumanPlayer(/* args */)
{
}

HumanPlayer::~HumanPlayer()
{
}

void HumanPlayer::play() 
{
    m_playAgain = true;
}



