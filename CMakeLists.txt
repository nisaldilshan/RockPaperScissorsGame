cmake_minimum_required(VERSION 3.1)

project(Test)

#add_compile_definitions(DISABLE_DEBUG_LOG)

add_executable(game main.cpp 
                    Game.cpp
                    Player.cpp 
                    HumanPlayer.cpp 
                    ComputerPlayer.cpp
                    Server.cpp
                    Client.cpp
                    Util.cpp
                )
