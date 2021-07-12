#include "Game.h"
#include "Util.h"

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
        player1 = std::make_unique<HumanPlayer>(true, m_isHost);
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
    Client& networkClient = Client::get();
    NetworkMessage inputMsg;
    inputMsg.type = MessageType::PlayerInput;
    memcpy(inputMsg.data, &se.playerOneInput, sizeof(Gesture));
    networkClient.sendData((const char*)&inputMsg, sizeof(NetworkMessage));

    // wait for ack
    char recvbuf1[DEFAULT_BUFLEN];
    int b1 = networkClient.recieveData(recvbuf1, DEFAULT_BUFLEN);
    NetworkMessage ackMsg1;
    memcpy(&ackMsg1, recvbuf1, sizeof(NetworkMessage));
    if (ackMsg1.type == MessageType::Ack)
        std::cout << "Acknowledged" <<std::endl;
    else
        Util::Error("Expected Ack message");

    //////////////////////////////////////////////////////////////

    m_running = player1->wantToPlayAgain();
    NetworkMessage playAgainInputMsg;
    playAgainInputMsg.type = MessageType::PlayerPlayAgainInput;
    memcpy(playAgainInputMsg.data, &m_running, sizeof(bool));
    networkClient.sendData((const char*)&playAgainInputMsg, sizeof(NetworkMessage));

    // wait for ack
    char recvbuf2[DEFAULT_BUFLEN];
    int b2 = networkClient.recieveData(recvbuf2, DEFAULT_BUFLEN);
    NetworkMessage ackMsg2;
    memcpy(&ackMsg2, recvbuf2, sizeof(NetworkMessage));
    if (ackMsg2.type == MessageType::Ack)
        std::cout << "Acknowledged" <<std::endl;
    else
        Util::Error("Expected Ack message");

    //////////////////////////////////////////////////////////////

    // get round winner from server
    char recvbuf3[DEFAULT_BUFLEN];
    int b3 = networkClient.recieveData(recvbuf3, DEFAULT_BUFLEN);
    NetworkMessage msg;
    memcpy(&msg, recvbuf3, sizeof(NetworkMessage));

    if (msg.type == MessageType::Winner)
        memcpy(&se.res, &msg.data, sizeof(RoundResult));
    else
        Util::Error("Expected Winner message");


    // send Ack
    NetworkMessage sendAckMsg;
    sendAckMsg.type = MessageType::Ack;
    networkClient.sendData((const char*)&sendAckMsg, sizeof(NetworkMessage));


    //////////////////////////////////////////////////////////////
}

void Game::endGameJoiner() 
{
    //get summary from server
    Client& networkClient = Client::get();
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
        Util::Error("Expected GameSummary message");

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

