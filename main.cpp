#include "Player.h"
#include <memory>
#include <iostream>
#include <list>

enum class RoundResult { 
    PlayerOneWins, 
    PlayerTwoWins,
    Draw
}; 

struct ScoreEntry
{
    Gesture playerOneInput;
    Gesture playerTwoInput;
    RoundResult res;
};


class Game
{
public:
    Game(/* args */);
    ~Game();
    void setup();
    void run();
    void exit();

private:
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    bool m_running = true;
    std::list<ScoreEntry> scores;
    bool m_isSinglePlayer = true;
};

Game::Game(/* args */)
{
}

Game::~Game()
{
}

void Game::setup() 
{
    if (m_isSinglePlayer)
    {
        player1 = std::make_unique<HumanPlayer>();
        player2 = std::make_unique<ComputerPlayer>();
    }
    else
    {
        player1 = std::make_unique<HumanPlayer>(true);
        player2 = std::make_unique<HumanPlayer>(false);
    }
}

void Game::run() 
{
    while (m_running)
    {
        ScoreEntry se;
        se.playerOneInput = player1->play();
        se.playerTwoInput = player2->play();
        
        if (se.playerOneInput == se.playerTwoInput)
            se.res = RoundResult::Draw;
        else
        {
            if (se.playerOneInput == Gesture::Rock && se.playerTwoInput == Gesture::Paper || 
            se.playerOneInput == Gesture::Paper && se.playerTwoInput == Gesture::Scissor || 
            se.playerOneInput == Gesture::Scissor && se.playerTwoInput == Gesture::Rock)
                se.res = RoundResult::PlayerTwoWins;
            else 
                se.res = RoundResult::PlayerOneWins;
        }

        scores.push_back(se);
        std::cout << (se.res == RoundResult::Draw ? "Draw" : se.res == RoundResult::PlayerOneWins ? "PlayerOneWins" : "PlayerTwoWins") << std::endl;


        m_running = player1->wantToPlayAgain() && player2->wantToPlayAgain();
    }

    // send summary to both players
    int round = 1;
    for (auto score : scores)
    {
        std::cout << "Round" << round++ << " : " << (score.res == RoundResult::Draw ? "Draw" : score.res == RoundResult::PlayerOneWins ? "PlayerOneWins" : "PlayerTwoWins") << std::endl;
    }
}



int main()
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->setup();
    game->run();
    return 0;
}