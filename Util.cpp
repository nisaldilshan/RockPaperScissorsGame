#include "Util.h"
#include "Network.h"
#include <iostream>
#include <exception>

void Util::Log(const std::string&& logString) 
{
#ifndef DISABLE_DEBUG_LOG
    std::cout << "LOG:" << logString << std::endl;
#endif
}

void Util::Error(const std::string&& errorString) 
{
    std::cerr << "ERROR:" << errorString << std::endl;
    __debugbreak();
	std::terminate();
}

std::string Util::alignString( Position pos, std::string s, int linelength )
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

std::string Util::getUserIPAddress() 
{
    std::cout << "Do you want to connect to localhost (Default-" << DEFAULT_IP << ") ? (y/n) : "; 
    char in;
    std::cin >> in;
    while (in != 'Y' && in != 'N' && in != 'y' && in != 'n')
    {
        std::cout << "Do you want to connect to localhost (Default-" << DEFAULT_IP << ") ? (y/n) : "; 
        std::cin >> in;
    }
    std::string ipAddress(DEFAULT_IP);
    if (in == 'N' || in == 'n')
    {
        std::cout << "Enter IP address : "; 
        std::cin >> ipAddress;
        std::cout << "Entered IP address is: " << ipAddress <<std::endl; 
    }

    return ipAddress;
}


std::string Util::getGestureString(int gesture) 
{
    if (gesture == 1)
        return std::string("Rock");
    if (gesture == 2)
        return std::string("Paper");
    if (gesture == 3)
        return std::string("Scissor");
    else
        return std::string("None");
}