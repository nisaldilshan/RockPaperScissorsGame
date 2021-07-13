#pragma once
#include <string>

namespace Util {

void Log(const std::string&& logString);
void Error(const std::string&& errorString);

enum class Position{ LEFT, CENTRE, RIGHT };
std::string alignString( Position pos, std::string s, int linelength );

std::string getUserIPAddress();

}