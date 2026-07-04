#include "logger/AppLogger.h"
#include <fstream>
#include <iostream>

// private
void AppLogger::log(LogLevel level, const std::string &msg)
{
    if (level < m_level)
    {
        return;
    }

    std::string out = "[" + timestamp() + "] [" + levelToString(level) + "] " + msg + "\n";

    std::cout << out;

    if (m_file.is_open())
    {
        m_file << out; // write to file
    }
}

std::string AppLogger::levelToString(LogLevel level) const
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

std::string AppLogger::timestamp() const
{
}

// public
void AppLogger::setLevel(LogLevel level)
{
    m_level = level;
}

void AppLogger::setFile(const std::string &path)
{
    std::ofstream file(path);

    file.close();
}

void AppLogger::debug(const std::string &msg)
{
}

void AppLogger::info(const std::string &msg)
{
}

void AppLogger::warn(const std::string &msg)
{
}

void AppLogger::error(const std::string &msg)
{
}
