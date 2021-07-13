#pragma once
#include <string>

#ifndef _WIN32
    #error "Unknown platform! - (Supported platforms:Windows)"
#endif

namespace Util {

void Log(const std::string&& logString);
void Error(const std::string&& errorString);

constexpr int LINELENGTH = 80;
enum class Position{ LEFT, CENTRE, RIGHT };
std::string alignString( Position pos, std::string s, int linelength );

std::string getUserIPAddress();

std::string getGestureString(int gesture);

}