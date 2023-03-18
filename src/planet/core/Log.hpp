#pragma once
#include <string>

#define LOG(mes) \
Logger::log(Logger::Level::INFO, mes)

#define LOG_DEBUG(mes) \
Logger::log(Logger::Level::DEBUG, mes)

#define LOG_TRACE(mes) \
Logger::log(Logger::Level::TRACE, mes)

#define LOG_ERROR(mes) \
Logger::log(Logger::Level::ERROR, mes)

#define tstr std::to_string

class Logger
{
public:
    enum class Level
    {
        NONE   = 0,
        ERROR  = 1,
        INFO   = 2,
        DEBUG  = 3,
        TRACE  = 4 
    };

    static void log(Level _level, std::string_view _message);

private:
    static Level ms_logLevel;
};

