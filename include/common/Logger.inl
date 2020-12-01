
#include "Logger.h"

template<class ...Args>
void Logger::verbose(Args && ...args, const Position & pos) {
    log(LogLevel::Verbose, args..., pos);
}

template<class ...Args>
void Logger::debug(Args && ...args, const Position & pos) {
    log(LogLevel::Debug, args..., pos);
}

template<class ...Args>
void Logger::info(Args && ...args, const Position & pos) {
    log(LogLevel::Info, args..., pos);
}

template<class ...Args>
void Logger::warn(Args && ...args, const Position & pos) {
    log(LogLevel::Warn, args..., pos);
}

template<class ...Args>
void Logger::error(Args && ...args, const Position & pos) {
    log(LogLevel::Error, args..., pos);
}

template<class Arg, class ...Args>
void Logger::log(LogLevel level, Arg && first, Args && ...other, const Position & pos) {
    if (static_cast<uint8_t>(level) < static_cast<uint8_t>(options.level)) {
        return;
    }

    if (!pos.filename.empty() && pos.line > 0 && pos.column > 0) {
        pos.filename + ":" + std::to_string(pos.line) + ":" + std::to_string(pos.column) + ": ";
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
