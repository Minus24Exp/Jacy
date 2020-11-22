#ifndef LOGGER_H
#define LOGGER_H

#include "common/Options.h"
#include <iostream>
#include <map>

namespace jc::common {
    /**
     * Logger
     * Universal class for dev and prod logs.
     */
    class Logger {
    public:
        Logger(std::string _class, const LoggerOptions & options) : _class(std::move(_class)), options(options) {}

        LoggerOptions options;

        template<typename ...Args>
        void verbose(Args && ...args);

        template<typename ...Args>
        void debug(Args && ...args);

        template<typename ...Args>
        void info(Args && ...args);

        template<typename ...Args>
        void warn(Args && ...args);

        template<typename ...Args>
        void error(Args && ...args);

    private:
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
        void log(LogLevel level, Arg && first, Args && ...other);
    };

    #include "common/Logger.inl"
}

#endif // LOGGER_H
