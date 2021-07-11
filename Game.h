#include "Player.h"
#include "Client.h"
#include <memory>
#include <iostream>
#include <list>
#include <sstream>

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
    std::string getMatchSummary();
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    bool m_running = true;
    std::list<ScoreEntry> m_scores;
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

        m_scores.push_back(se);
        player1->announceWinner(se.res);
        player2->announceWinner(se.res);

        m_running = player1->wantToPlayAgain() && player2->wantToPlayAgain();
    }

    // send summary to both players
    auto summary = getMatchSummary();
    player1->announceSummary(summary);
    player2->announceSummary(summary);
}

void Game::runJoinee() 
{
    auto networkClient = std::make_unique<Client>();

    while (m_running)
    {
        Gesture playerOneInput = player1->play();
        NetworkMessage inputMsg;
        inputMsg.type = MessageType::PlayerInput;
        memcpy(inputMsg.data, &playerOneInput, sizeof(Gesture));
        networkClient->sendData((const char*)&inputMsg, sizeof(NetworkMessage));
        // wait for ack

        m_running = player1->wantToPlayAgain();
        NetworkMessage playAgainInputMsg;
        playAgainInputMsg.type = MessageType::PlayerPlayAgainInput;
        memcpy(playAgainInputMsg.data, &m_running, sizeof(bool));
        networkClient->sendData((const char*)&playAgainInputMsg, sizeof(NetworkMessage));
        // wait for ack

        // display round winner
        char recvbuf[DEFAULT_BUFLEN];
        int bytes = networkClient->recieveData(recvbuf, DEFAULT_BUFLEN);

        NetworkMessage msg;
        memcpy(&msg, recvbuf, sizeof(NetworkMessage));

        RoundResult roundRes;
        if (msg.type == MessageType::Winner)
            memcpy(&roundRes, &msg.data, sizeof(RoundResult));
        else
            __debugbreak();

        std::cout << (roundRes == RoundResult::Draw ? "Draw" : roundRes == RoundResult::PlayerOneWins ? "PlayerOneWins" : "PlayerTwoWins") << std::endl;
    }

    // wait for server to send summary and display summary
    // destroy client

}

std::string Game::getMatchSummary() 
{
    int round = 1;
    std::stringstream ss;
    for (auto score : m_scores)
    {
        ss << "Round" << round++ << " : " << (score.res == RoundResult::Draw ? "Draw" : score.res == RoundResult::PlayerOneWins ? "PlayerOneWins" : "PlayerTwoWins") << std::endl;
    }
    return ss.str();
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