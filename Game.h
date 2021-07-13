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
    ~Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;
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
    std::pair<std::string, std::string> getMatchSummary();
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    bool m_running = true;
    std::list<ScoreEntry> m_scores;
    bool m_isSinglePlayer = true;
    bool m_isHost = true;
    std::function<void(ScoreEntry)> m_applyGameLogicFunc;
    std::function<void()> m_endGameFunc;
};

