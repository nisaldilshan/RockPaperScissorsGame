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
    void endGame();
    void applyGameLogicHost(ScoreEntry& se);
    void endGameHost();
    void applyGameLogicJoiner(ScoreEntry& se);
    void endGameJoiner();
    std::string getMatchSummary();
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    bool m_running = true;
    std::list<ScoreEntry> m_scores;
    bool m_isSinglePlayer = true;
    bool m_isHost = true;
    std::function<void(ScoreEntry)> m_applyGameLogicFunc;
    std::function<void()> m_endGameFunc;
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
            player2 = std::make_unique<HumanPlayer>(false, m_isHost);

        player1 = std::make_unique<HumanPlayer>(true, m_isHost);
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
        applyGameLogicHost(se);
        m_running = player2->wantToPlayAgain() && player1->wantToPlayAgain();

        player1->announceWinner(se.res);
        player2->announceWinner(se.res);
    }

    endGameHost();
}

void Game::runJoinee() 
{
    while (m_running)
    {
        ScoreEntry se;
        se.playerOneInput = player1->play();

        applyGameLogicJoiner(se);

        player1->announceWinner(se.res);
    }

    endGameJoiner();
}

void Game::applyGameLogicHost(ScoreEntry& se) 
{
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
}

void Game::endGameHost() 
{
    auto summary = getMatchSummary();

    // send summary to both players
    player1->announceSummary(summary);
    player2->announceSummary(summary);
}

void Game::applyGameLogicJoiner(ScoreEntry& se) 
{
    auto networkClient = Client::get();
    NetworkMessage inputMsg;
    inputMsg.type = MessageType::PlayerInput;
    memcpy(inputMsg.data, &se.playerOneInput, sizeof(Gesture));
    networkClient.sendData((const char*)&inputMsg, sizeof(NetworkMessage));
    // wait for ack

    m_running = player1->wantToPlayAgain();
    NetworkMessage playAgainInputMsg;
    playAgainInputMsg.type = MessageType::PlayerPlayAgainInput;
    memcpy(playAgainInputMsg.data, &m_running, sizeof(bool));
    networkClient.sendData((const char*)&playAgainInputMsg, sizeof(NetworkMessage));
    // wait for ack

    // get round winner from server
    char recvbuf[DEFAULT_BUFLEN];
    int bytes = networkClient.recieveData(recvbuf, DEFAULT_BUFLEN);
    NetworkMessage msg;
    memcpy(&msg, recvbuf, sizeof(NetworkMessage));

    if (msg.type == MessageType::Winner)
        memcpy(&se.res, &msg.data, sizeof(RoundResult));
    else
        __debugbreak();


    //send Ack
    NetworkMessage sendAckMsg;
    sendAckMsg.type = MessageType::Ack;
    networkClient.sendData((const char*)&sendAckMsg, sizeof(NetworkMessage));
}

void Game::endGameJoiner() 
{
    //get summary from server
    auto networkClient = Client::get();
    char recvbuf[DEFAULT_BUFLEN];
    int bytes = networkClient.recieveData(recvbuf, DEFAULT_BUFLEN);
    NetworkMessage summaryMsg;
    memcpy(&summaryMsg, recvbuf, sizeof(NetworkMessage));

    std::string summary;
    summary.reserve(summaryMsg.length);
    const char* s = summary.c_str();
    if (summaryMsg.type == MessageType::GameSummary)
        summary = std::string(summaryMsg.data, summaryMsg.length); //memcpy(&s, &summaryMsg.data, summaryMsg.length);
    else
        __debugbreak();

    player1->announceSummary(summary);
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