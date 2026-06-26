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

/// @brief removes anything after a semicolon
/// @param line 
/// @return {std::string} - line
std::string GcodeParser::stripComment(const std::string &line)
{
    auto pos = line.find(';');
    if (pos != std::string::npos)
    {
        return line.substr(0, pos);
    }

    return line;
}

/// @brief break gcode formatting "G1 X10.5 Y-3.2 F300" into {G:1, X:10.5, Y:-3.2, F:300}
/// @param line 
/// @return {std::unordered_map} - words
std::unordered_map<char, double> GcodeParser::tokenise(const std::string &line)
{
    std::unordered_map<char, double> words;
    std::istringstream ss(line);
    char letter;
    double value;

    while (ss >> letter >> value)
    {
        letter = std::toupper(letter);
        words[letter] = value;
    }

    return words;
}

/// @brief build 
/// @param gCode 
/// @param words 
/// @return {std::optional<GCommand>} 
std::optional<GCommand> GcodeParser::buildCommand(int gCode, const std::unordered_map<char, double> &words)
{
    
}

/// @brief parse a single line
/// @param line 
/// @return {std::optional<GCommand>} line
std::optional<GCommand> GcodeParser::parseLine(const std::string &line)
{

} 

std::vector<GCommand> parseFile(const std::string &path);   // parse an entire file