#ifndef LOGGER_H
#define LOGGER_H

#include "common/Options.h"
#include <iostream>
#include <map>

/**
 * Logger
 * Universal class for dev and prod logs.
 */
class Logger {
public:
    Logger(std::string _class, const LoggerOptions & options);

    void verbose(const std::string & msg);
    void debug(const std::string & msg);
    void info(const std::string & msg);
    void warn(const std::string & msg);
    void error(const std::string & msg);

private:
    LoggerOptions options;
    std::string _class;

    const std::map<LogLevel, std::string> level_names = {
        {LogLevel::Verbose, "verbose"},
        {LogLevel::Debug, "debug"},
        {LogLevel::Info, "info"},
        {LogLevel::Warn, "warn"},
        {LogLevel::Error, "error"},
    };

    const std::map<LogLevel, Color> level_colors = {
        {LogLevel::Verbose, Color::Magenta},
        {LogLevel::Debug, Color::Blue},
        {LogLevel::Info, Color::Green},
        {LogLevel::Warn, Color::Yellow},
        {LogLevel::Error, Color::Red},
    };

    const std::map<Color, std::string> colors = {
        {Color::Red, "\033[1;31m"},
        {Color::Green, "\033[1;32m"},
        {Color::Blue, "\033[1;34m"},
        {Color::Yellow, "\033[1;33m"},
        {Color::Magenta, "\033[1;35m"},
        {Color::Cyan, "\033[1;36m"},
    };

    const std::string ansi_reset = "\033[1;0m";

    void log(LogLevel level, const std::string & msg);
};

#endif // LOGGER_H
