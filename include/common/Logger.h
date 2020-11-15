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
    Logger(std::string _class, const LoggerOptions & options) : _class(std::move(_class)), options(options) {}

    template<typename ...Args>
    void verbose(Args && ...args) {
        log(LogLevel::Verbose, args...);
    }

    template<typename ...Args>
    void debug(Args && ...args) {
        log(LogLevel::Debug, args...);
    }

    template<typename ...Args>
    void info(Args && ...args) {
        log(LogLevel::Info, args...);
    }

    template<typename ...Args>
    void warn(Args && ...args) {
        log(LogLevel::Warn, args...);
    }

    template<typename ...Args>
    void error(Args && ...args) {
        log(LogLevel::Error, args...);
    }

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

    template<typename Arg, typename ...Args>
    void log(LogLevel level, Arg && first, Args && ...other) {
        if (static_cast<uint8_t>(level) < static_cast<uint8_t>(options.level)) {
            return;
        }

        if (options.log_class) {
            if (options.static_last_class) {
                static std::string last_class;
                if (last_class != _class) {
                    std::cout << "\t[" << _class << "]" << std::endl;
                    last_class = _class;
                }
            } else {
                std::cout << _class << " ";
            }
        }

        if (options.log_level) {
            if (options.colorize) {
                std::cout << colors.at(level_colors.at(level));
            }
            std::cout << level_names.at(level) << ": ";
            if (options.colorize) {
                std::cout << ansi_reset;
            }
        }

        std::cout << std::forward<Arg>(first);

        ((std::cout << ' ' << std::forward<Args>(other)), ...);

        std::cout << std::endl;
    }
};

#endif // LOGGER_H
