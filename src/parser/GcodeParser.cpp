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
/// produces a map of the gcode to the value e.g. G -> 1
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

/// @brief build the parsed GCode into a C++ GCommand
/// fills in the corresponding struct with the gcode
/// @param gCode 
/// @param words 
/// @return {std::optional<GCommand>} 
std::optional<GCommand> GcodeParser::buildCommand(int gCode, const std::unordered_map<char, double> &words)
{
    // TODO
}

/// @brief parse a single line
/// @param line 
/// @return {std::optional<GCommand>} line
std::optional<GCommand> GcodeParser::parseLine(const std::string &line)
{
    std::string cleaned_line = stripComment(line);

    // check if line is blank
    if (cleaned_line.find_first_not_of(" \t\r\n") == std::string::npos)
    {
        return std::nullopt; // return empty optional
    }

    auto words = tokenise(cleaned_line);

    if (words.empty())
    {
        return std::nullopt;
    }

    // if G word is found, handle motion command
    if (words.count('G'))
    {
        int gCode = static_cast<int>(words.at('G'));

        // TODO
    }

    // if M word is found, handle machine command
    if (words.count('M'))
    {
        // TODO
    }


    return std::nullopt;
} 

/// @brief parse an entire file
/// @param path 
/// @return {std::vector<GCommand>} full vector of converted Gcode to C++ Gcommands
std::vector<GCommand> parseFile(const std::string &path)
{
    // TODO
}
