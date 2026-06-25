#pragma once
#include "GCommand.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

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
}
