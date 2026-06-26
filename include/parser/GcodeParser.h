#pragma once
#include "GCommand.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

/*
gcode list:

Code  Meaning
G0  Rapid move (as fast as possible)
G1  Linear move (at a set feed rate)
G2  Arc move, clockwise
G3  Arc move, counter-clockwise
G90 Absolute positioning mode
G91 Incremental positioning mode\
M3  Spindle on
M5  Spindle off
M30 End of program
*/
class GcodeParser
{
  private:
    // modal states - stays active until it changes
    bool m_absoluteMode{true}; // G90 = true, G91 = false
    double m_currentFeedrate{0.0};

    // helper functions
    std::string stripComment(const std::string &line);
    std::unordered_map<char, double> tokenise(const std::string &line);
    std::optional<GCommand> buildCommand(int gCode, const std::unordered_map<char, double> &words);

  public:
    GcodeParser();
    std::optional<GCommand> parseLine(const std::string &line); // parse a single line
    std::vector<GCommand> parseFile(const std::string &path);   // parse an entire file
};
