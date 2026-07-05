#include "logger/AppLogger.h"
#include <chrono>
#include <ctime>
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
    auto now = std::chrono::system_clock::now();

    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::string ts = std::ctime(&t); // formatting to string
    ts.pop_back();                   // remove trailing newline char

    return ts;
}

// public
void AppLogger::setLevel(LogLevel level)
{
    m_level = level;
}

/// @brief opens the file for the lifetime of the logger
/// @param path
void AppLogger::setFile(const std::string &path)
{
    m_file.open(path);

    if (!m_file.is_open())
    {
        std::cerr << "AppLogger: could not open log file: " << path << '\n';
    }
}

AppLogger &AppLogger::get()
{
    static AppLogger instance;

    return instance;
}

void AppLogger::debug(const std::string &msg)
{
    log(LogLevel::DEBUG, msg);
}

void AppLogger::info(const std::string &msg)
{
    log(LogLevel::INFO, msg);
}

void AppLogger::warn(const std::string &msg)
{
    log(LogLevel::WARN, msg);
}

void AppLogger::error(const std::string &msg)
{
    log(LogLevel::ERROR, msg);
}
