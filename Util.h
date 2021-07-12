#pragma once
#include <string>

namespace Util {

void Log(const char* logString);
void Error(const char* errorString);

enum class Position{ LEFT, CENTRE, RIGHT };
std::string alignString( Position pos, std::string s, int linelength );

}