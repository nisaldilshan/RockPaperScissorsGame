#pragma once
#include <functional>
#include <string>

enum class Gesture { 
    Rock = 1, 
    Paper, 
    Scissor,
    None
};

enum class RoundResult { 
    PlayerOneWins, 
    PlayerTwoWins,
    Draw
}; 

class Player
{
public:
    Player() = default;
    virtual ~Player() = default;
    Gesture play();
    bool wantToPlayAgain();
    virtual void announceWinner(RoundResult winner, bool playAgain) = 0;
    virtual void announceSummary(std::string summary) = 0;
private:
    virtual Gesture getInput() = 0;
    virtual bool getPlayAgainInput() = 0;
    void processInput(Gesture input);
};

class ComputerPlayer :public Player
{
public:
    ComputerPlayer(/* args */);
    ~ComputerPlayer() = default;
    virtual void announceWinner(RoundResult winner, bool playAgain) override;
    virtual void announceSummary(std::string summary) override;
private:
    virtual Gesture getInput();
    virtual bool getPlayAgainInput();
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(bool isLocal, bool isHost);
    ~HumanPlayer() = default;
    virtual void announceWinner(RoundResult winner, bool playAgain) override;
    virtual void announceSummary(std::string summary) override;
private:
    virtual Gesture getInput() override;
    virtual bool getPlayAgainInput() override;
    std::function<Gesture()> m_getInputFunc;
    std::function<bool()> m_getPlayAgainInputFunc;
    std::function<void(RoundResult, bool)> m_announceWinnerFunc;
    std::function<void(std::string)> m_announceSummaryFunc;
};
