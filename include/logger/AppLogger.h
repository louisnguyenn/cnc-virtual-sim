#pragma once
#include <fstream>
#include <string>

enum class LogLevel
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
};

class AppLogger
{
  private:
    LogLevel m_level{LogLevel::INFO};
    std::ofstream m_file;

    // private logger - singleton pattern (only one logger for the whole application)
    AppLogger() = default;

    void log(LogLevel level, const std::string &msg);
    std::string levelToString(LogLevel level) const;
    std::string timestamp() const;

  public:
    static AppLogger &get(); // singleton

    // setters
    void setLevel(LogLevel level);
    void setFile(const std::string &path);

    void debug(const std::string &msg);
    void info(const std::string &msg);
    void warn(const std::string &msg);
    void error(const std::string &msg);
};
