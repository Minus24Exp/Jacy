#include "Logger.h"
#include "Exception.h"

template<class ...Args>
void Logger::verbose(Args && ...args) {
    log(LogLevel::Verbose, args...);
}

template<class ...Args>
void Logger::debug(Args && ...args) {
    log(LogLevel::Debug, args...);
}

template<class ...Args>
void Logger::info(Args && ...args) {
    log(LogLevel::Info, args...);
}

template<class ...Args>
void Logger::warn(Args && ...args) {
    log(LogLevel::Warn, args...);
}

template<class ...Args>
void Logger::error(Args && ...args) {
    log(LogLevel::Error, args...);
}

template<class Arg, class ...Args>
void Logger::log(LogLevel level, Arg && first, Args && ...other) {
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
            std::cout << Logger::ansi_reset;
        }
    }

    std::cout << std::forward<Arg>(first);

    ((std::cout << ' ' << std::forward<Args>(other)), ...);

    std::cout << std::endl;
}
