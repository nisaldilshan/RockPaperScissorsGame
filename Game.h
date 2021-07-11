#include "Player.h"
#include "Client.h"
#include "NetworkMessage.h"
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
    Game(int gameMode, int connectMode);
    ~Game();
    void setup();
    void run();
    void exit();

private:
    void runHost();
    void runJoinee();
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    bool m_running = true;
    std::list<ScoreEntry> scores;
    bool m_isSinglePlayer = true;
    bool m_isHost = true;
};

Game::Game(int gameMode, int connectMode)
{
    if (gameMode == 2)
        m_isSinglePlayer = false;

    if (connectMode == 2)
        m_isHost = false;
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
        if (m_isHost)
        {
            player1 = std::make_unique<HumanPlayer>(true);
            player2 = std::make_unique<HumanPlayer>(false);
        }
        else
        {
            player1 = std::make_unique<HumanPlayer>();
        }
    }
}

void Game::run() 
{
    if (m_isHost)
        runHost();
    else
        runJoinee();
}

void Game::runHost() 
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

void Game::runJoinee() 
{
    char recvbuf[DEFAULT_BUFLEN];
    auto networkClient = std::make_unique<Client>();

    while (m_running)
    {
        Gesture playerOneInput = player1->play();
        NetworkMessage msg;
        msg.type = MessageType::PlayerInput;
        memcpy(msg.data, &playerOneInput, sizeof(Gesture));
        networkClient->sendData((const char*)&msg, sizeof(NetworkMessage));

        m_running = player1->wantToPlayAgain();
        //networkClient->sendData(&msg, sizeof(NetworkMessage));

        // wait for ack

        // display round winner
    }

    // wait for server to send summary and display summary
    // destroy client

}

enum class Position{ LEFT, CENTRE, RIGHT };
std::string alignString( Position pos, std::string s, int linelength )
{
    size_t spaces = 0;
    std::string ret = "";
    switch( pos )
    {
        case Position::CENTRE: spaces = ( linelength - s.size() ) / 2; break;
        case Position::RIGHT : spaces =   linelength - s.size()      ; break;
    }
    if ( spaces > 0 ) 
        ret+= std::string( spaces, ' ' );
    return ret+s;
}