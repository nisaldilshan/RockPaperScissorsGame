#include "Game.h"
#include "Network.h"
#include "Util.h"

Game::Game(int gameMode, int connectMode)
{
    if (gameMode == 2)
        m_isSinglePlayer = false;

    if (connectMode == 2)
        m_isHost = false;
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

        player1->announceWinner(se.res, m_running);
        player2->announceWinner(se.res, m_running);
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

        player1->announceWinner(se.res, m_running);
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
    player1->announceSummary(summary.first);
    player2->announceSummary(summary.second);
}

void Game::applyGameLogicJoiner(ScoreEntry& se) 
{
    //send player input
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
        Util::Log("Acknowledged");
    else
        Util::Error("Expected Ack message");

    //////////////////////////////////////////////////////////////
    //send playAgain input
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
        Util::Log("Acknowledged");
    else
        Util::Error("Expected Ack message");

    //////////////////////////////////////////////////////////////

    // get round winner from server
    char recvbuf3[DEFAULT_BUFLEN];
    int b3 = networkClient.recieveData(recvbuf3, DEFAULT_BUFLEN);
    NetworkMessage msg;
    memcpy(&msg, recvbuf3, sizeof(NetworkMessage));

    if (msg.type == MessageType::Winner)
    {
        memcpy(&se.res, msg.data, sizeof(RoundResult));
        memcpy(&m_running, msg.data + sizeof(RoundResult), sizeof(bool));
    }
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

std::pair<std::string, std::string> Game::getMatchSummary() 
{
    int round = 1;
    std::stringstream ss1;
    std::stringstream ss2;
    std::string header(Util::LINELENGTH, '=' );
    size_t pOneScore = 0;
    size_t pTwoScore = 0;

    ss1 << header << std::endl << alignString( Util::Position::CENTRE, "Game Summary", Util::LINELENGTH ) << std::endl << header << std::endl;
    ss2 << header << std::endl << alignString( Util::Position::CENTRE, "Game Summary", Util::LINELENGTH ) << std::endl << header << std::endl;

    ss1 << "\t\t" << "You" << "\t\t" << "Opponent" << "\t" << "Winner" << std::endl;
    ss2 << "\t\t" << "Opponent" << "\t" << "You" << "\t\t" << "Winner" << std::endl;
    for (auto score : m_scores)
    {
        ss1 << "Round" << round << "\t\t";
        ss2 << "Round" << round << "\t\t";
        round++;

        ss1 << Util::getGestureString((int)score.playerOneInput) << "\t\t" << Util::getGestureString((int)score.playerTwoInput) << "\t\t";
        ss2 << Util::getGestureString((int)score.playerOneInput) << "\t\t" << Util::getGestureString((int)score.playerTwoInput) << "\t\t";

        if (score.res == RoundResult::PlayerOneWins)
        {
            ss1 << "You";
            ss2 << "Opponent";
            pOneScore++;
        } 
        else if (score.res == RoundResult::PlayerTwoWins)
        {
            ss1 << "Opponent";
            ss2 << "You";
            pTwoScore++;
        }
        else 
        {
            ss1 << "Draw";
            ss2 << "Draw";
        }

        ss1 << std::endl;
        ss2 << std::endl;
    }

    if (pOneScore > pTwoScore)
    {
        ss1 << header << std::endl << alignString( Util::Position::CENTRE, "You Won", Util::LINELENGTH ) << std::endl << header << std::endl;
        ss2 << header << std::endl << alignString( Util::Position::CENTRE, "You Lost", Util::LINELENGTH ) << std::endl << header << std::endl;
    }
    else if (pOneScore < pTwoScore)
    {
        ss1 << header << std::endl << alignString( Util::Position::CENTRE, "You Lost", Util::LINELENGTH ) << std::endl << header << std::endl;
        ss2 << header << std::endl << alignString( Util::Position::CENTRE, "You Won", Util::LINELENGTH ) << std::endl << header << std::endl;
    }
    else
    {
        ss1 << header << std::endl << alignString( Util::Position::CENTRE, "It's a Draw", Util::LINELENGTH ) << std::endl << header << std::endl;
        ss2 << header << std::endl << alignString( Util::Position::CENTRE, "It's a Draw", Util::LINELENGTH ) << std::endl << header << std::endl;
    }
    

    return {ss1.str(), ss2.str()};
}

