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

/// @brief helper, instant look up in map
/// @param c
/// @param words
/// @return value (double)
double get(char c, const std::unordered_map<char, double> &words)
{
    return words.count(c) ? words.at(c) : 0.0;
}

/// @brief build the parsed GCode into a C++ GCommand
/// fills in the corresponding struct with the gcode
/// @param gCode
/// @param words
/// @return {std::optional<GCommand>}
std::optional<GCommand> GcodeParser::buildCommand(int gCode, const std::unordered_map<char, double> &words)
{
    double feedrate = words.count('F') ? words.at('F') : m_currentFeedrate;

    /*
        G0  Rapid move (as fast as possible)
        G1  Linear move (at a set feed rate)
    */
    if (gCode == 0 || gCode == 1)
    {
        LinearMove lin_move;

        lin_move.x = words.count('X') ? get('X', words) : m_currentX;
        lin_move.y = words.count('Y') ? get('Y', words) : m_currentY;
        lin_move.z = words.count('Z') ? get('Z', words) : m_currentZ;
        lin_move.feedrate = feedrate;

        // G0 = true (fast), G1 = false (controlled)
        if (gCode == 0)
        {
            lin_move.rapid = true;
        }
        else if (gCode == 1)
        {
            lin_move.rapid = false;
        }

        m_currentX = lin_move.x;
        m_currentY = lin_move.y;
        m_currentZ = lin_move.z;

        return lin_move;
    }

    /*
        G2  Arc move, clockwise
        G3  Arc move, counter-clockwise
    */
    if (gCode == 2 || gCode == 3)
    {
        ArcMove arc;

        arc.x = words.count('X') ? get('X', words) : m_currentX;
        arc.y = words.count('Y') ? get('Y', words) : m_currentY;
        arc.z = words.count('Z') ? get('Z', words) : m_currentZ;
        arc.i = get('I', words);
        arc.j = get('J', words);
        arc.k = get('K', words);
        arc.feedrate = feedrate;

        // G2 = true, G3 = false (counterclockwise)
        if (gCode == 2)
        {
            arc.clockwise = true;
        }
        else if (gCode == 3)
        {
            arc.clockwise = false;
        }

        m_currentX = arc.x;
        m_currentY = arc.y;
        m_currentZ = arc.z;

        return arc;
    }

    if (gCode == 4)
    {
        return DwellCmd{get('P', words)};
    }

    std::cerr << "Unknown G-code: G" << gCode << '\n'; // unknown g-code
    return std::nullopt;
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

    auto words = tokenise(cleaned_line); // tokenise a word

    if (words.empty())
    {
        return std::nullopt; // no word
    }

    // if G word is found, handle motion command
    if (words.count('G'))
    {
        int gCode = static_cast<int>(words.at('G'));

        if (gCode == 90)
        {
            m_absoluteMode = true;
            return std::nullopt;
        }
        else if (gCode == 91)
        {
            m_absoluteMode = false;
            return std::nullopt;
        }

        if (words.count('F'))
        {
            m_currentFeedrate = words.at('F'); // set feedrate
        }

        return buildCommand(gCode, words);
    }

    // if M word is found, handle machine command
    if (words.count('M'))
    {
        int mCode = static_cast<int>(words.at('M'));

        if (mCode == 3)
        {
            return SpindleCmd{true, words.count('S') ? (int)words.at('S') : 1000};
        }
        else if (mCode == 5)
        {
            return SpindleCmd{false, 0};
        }
        else if (mCode == 30)
        {
            return ProgramEnd{};
        }
    }

    return std::nullopt;
}

/// @brief parse an entire file
/// @param path
/// @return {std::vector<GCommand>} full vector of converted Gcode to C++ Gcommands
std::vector<GCommand> GcodeParser::parseFile(const std::string &path)
{
    std::vector<GCommand> commands;
    std::string line;

    // open file
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << path << '\n';
        return {}; // return empty vector
    }

    while (std::getline(file, line))
    {
        auto cmd = parseLine(line);

        if (cmd.has_value())
        {
            commands.push_back(*cmd);
        }
    }

    // std::cout << "Parsed " << commands.size() << " commands from " << path << '\n';

    return commands;
}
