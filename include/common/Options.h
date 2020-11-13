#ifndef OPTIONS_H
#define OPTIONS_H

#include <cstdint>

// Color is used by logger and can be set in Options //
enum class Color {
    Red,
    Green,
    Blue,
    Yellow,
    Magenta,
    Cyan,
};

// LogLevel //
// uint8_t because we need to know the priority
enum class LogLevel : uint8_t {
    Verbose,
    Debug,
    Info,
    Warn,
    Error,
};


struct LoggerOptions {
    bool log_class{true};
    bool log_level{true};
    bool static_last_class{true};
    bool colorize{true};
};

// Jacy main options //
struct JacyOptions {
    bool debug{false};

    LoggerOptions log = {
        .log_class = false, // Jacy main class does not provide class name
    };
};

// Lexer //
struct LexerOptions {};

// Parser //
struct ParserOptions {
    bool log_parsing_entity{true};

    LoggerOptions log = {};
};

// Compiler //
struct CompilerOptions {
    bool print_compiling_opcode{true};

    LoggerOptions log = {};
};

#endif // OPTIONS_H
