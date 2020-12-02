#include "common/Logger.h"

namespace jc::common {
    const std::map<LogLevel, std::string> Logger::level_names = {
        {LogLevel::Verbose, "verbose"},
        {LogLevel::Debug, "debug"},
        {LogLevel::Info, "info"},
        {LogLevel::Warn, "warn"},
        {LogLevel::Error, "error"},
    };

    const std::map<LogLevel, Color> Logger::level_colors = {
        {LogLevel::Verbose, Color::Magenta},
        {LogLevel::Debug, Color::Blue},
        {LogLevel::Info, Color::Green},
        {LogLevel::Warn, Color::Yellow},
        {LogLevel::Error, Color::Red},
    };

    const std::map<Color, std::string> Logger::colors = {
        {Color::Red, "\033[1;31m"},
        {Color::Green, "\033[1;32m"},
        {Color::Blue, "\033[1;34m"},
        {Color::Yellow, "\033[1;33m"},
        {Color::Magenta, "\033[1;35m"},
        {Color::Cyan, "\033[1;36m"},
    };

    const std::string Logger::ansi_reset = "\033[1;0m";

    std::string Logger::format_error(const JacyException & exception) {
        const auto & message = exception.what();
        const auto & pos = exception.pos;

        std::string formatted;
        if (!pos.filename.empty() && pos.line > 0 && pos.column > 0) {
            formatted = pos.filename + ":" + std::to_string(pos.line) + ":" + std::to_string(pos.column) + ": ";
        }

        formatted += colors.at(level_colors.at(LogLevel::Error));
        formatted += "Error: ";
        formatted += message;
        formatted += Logger::ansi_reset;

        return formatted;
    }
}
