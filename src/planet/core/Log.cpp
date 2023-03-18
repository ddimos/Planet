#include "core/Log.hpp"
#include <iostream>

Logger::Level Logger::ms_logLevel = Logger::Level::INFO;

void Logger::log(Level _level, std::string_view _message)
{
    if (ms_logLevel < _level)
        return;

    switch (_level)
    {
    case Level::TRACE:
    case Level::DEBUG:
    case Level::INFO:
    {
        std::cout  << _message << "\n";
        break;
    }
    case Level::ERROR:
    {
        std::cout << " [ERROR] " << _message << std::endl;
        break;
    }
    default:
        break;
    }
}