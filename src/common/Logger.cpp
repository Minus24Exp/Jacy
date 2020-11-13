#include "common/Logger.h"

Logger::Logger(const std::string & _class, const Options & options) : _class(_class), options(options) {}

void Logger::log(LogLevel level, const std::string & msg) {
    if (static_cast<uint8_t>(level) < static_cast<uint8_t>(log_level)) {
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

    std::cout << msg << std::endl;
}

void Logger::verbose(const std::string & msg) {
    log(LogLevel::Verbose, msg);
}

void Logger::debug(const std::string & msg) {
    log(LogLevel::Debug, msg);
}

void Logger::info(const std::string & msg) {
    log(LogLevel::Info, msg);
}

void Logger::warn(const std::string & msg) {
    log(LogLevel::Warn, msg);
}

void Logger::error(const std::string & msg) {
    log(LogLevel::Error, msg);
}