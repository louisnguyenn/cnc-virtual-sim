#include "logger/AppLogger.h"

// private
void AppLogger::log(LogLevel level, const std::string &msg)
{
}

std::string AppLogger::levelToString(LogLevel level) const
{
}

std::string AppLogger::timestamp() const
{
}

// public
void AppLogger::setLevel(LogLevel level)
{
}

void AppLogger::setFile(const std::string &path)
{
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
