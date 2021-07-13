#include "Game.h"
#include "Util.h"

int main()
{
    std::string header(Util::LINELENGTH, '=' );
    std::cout << header << std::endl;
    std::cout << alignString( Util::Position::CENTRE, "Rock Paper Scissors Game", Util::LINELENGTH ) << std::endl;
    std::cout << header << std::endl;
    std::cout << alignString( Util::Position::LEFT, "Select Game Mode : (1.One Player/ 2.Two Player) - ", Util::LINELENGTH );

    int gameMode = 0;
    int connectMode = 0;

    while (!(gameMode == 1 || gameMode == 2))
        std::cin >> gameMode;
    
    if (gameMode == 2)
    {
        std::cout << alignString( Util::Position::LEFT, "Select Connect Mode : (1.HostGame/ 2.JoinGame) - ", Util::LINELENGTH );
        while (!(connectMode == 1 || connectMode == 2))
            std::cin >> connectMode;
    }

    std::unique_ptr<Game> game = std::make_unique<Game>(gameMode, connectMode);
    game->setup();
    game->run();
    return 0;
}