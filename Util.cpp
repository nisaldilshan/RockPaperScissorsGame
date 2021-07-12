#include "Util.h"
#include <iostream>
#include <exception>

void Util::Log(const char* logString) 
{
    std::cerr << "LOG:" << logString << std::endl;
}

void Util::Error(const char* errorString) 
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