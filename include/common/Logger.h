#ifndef LOGGER_H
#define LOGGER_H

#include "common/Options.h"
#include "Exception.h"

#include <iostream>
#include <map>
#include <sstream>

namespace jc::common {
    /**
     * Logger
     * Universal class for dev and prod logs.
     */
    class Logger {
    public:
        Logger(std::string _class, const LoggerOptions & options) : _class(std::move(_class)), options(options) {}

        LoggerOptions options;

        template<class ...Args>
        void verbose(Args && ...args);

        template<class ...Args>
        void debug(Args && ...args);

        template<class ...Args>
        void info(Args && ...args);

        template<class ...Args>
        void warn(Args && ...args);

        template<class ...Args>
        void error(Args && ...args);

        static std::string format_error(const JacyException & exception);

        static const std::map<LogLevel, std::string> level_names;
        static const std::map<LogLevel, Color> level_colors;
        static const std::map<Color, std::string> colors;
        static const std::string ansi_reset;

    private:
        std::string _class;

        template<class Arg, class ...Args>
        void log(LogLevel level, Arg && first, Args && ...other);
    };

    #include "common/Logger.inl"
}

#endif // LOGGER_H
