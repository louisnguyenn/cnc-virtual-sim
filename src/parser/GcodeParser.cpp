#include "parser/GcodeParser.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

/*
format:
"G1 X50 Y0 F300"
"G2 X0 Y50 I-25"
"M30"
*/

GcodeParser::GcodeParser()
{
}

std::string GcodeParser::stripComment(const std::string &line)
{
    auto pos = line.find(';');
    if (pos != std::string::npos)
    {
        return line.substr(0, pos);
    }

    return line;
}

std::unordered_map<char, double> tokenise(const std::string &line);
std::optional<GCommand> buildCommand(int gCode, const std::unordered_map<char, double> &words);
std::optional<GCommand> parseLine(const std::string &line); // parse a single line
std::vector<GCommand> parseFile(const std::string &path);   // parse an entire file