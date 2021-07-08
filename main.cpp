#include "Player.h"
#include <memory>

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
};

Game::Game(/* args */)
{
}

Game::~Game()
{
}

void Game::setup() 
{
    //if (singleplayer)
    {
        player1 = std::make_unique<HumanPlayer>();
        player2 = std::make_unique<ComputerPlayer>();
    }
}

void Game::run() 
{
    while (m_running)
    {
        player1->play();
        player2->play();
        m_running = player1->wantToPlayAgain() && player2->wantToPlayAgain();
    }
}



int main()
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->setup();
    game->run();
    return 0;
}