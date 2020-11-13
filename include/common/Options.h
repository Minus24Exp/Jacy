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

struct Options {
    // Logger options //
    bool log_class{true};
    bool log_level{true};
    bool static_last_class{true};
    bool colorize{true};
};

// Lexer //
struct LexerOptions : Options {

};

// Parser //
struct ParserOptions : Options {
    bool log_parsing_entity{true};
};

// Compiler //
struct CompilerOptions : Options {
    bool print_compiling_opcode{true};
};

#endif // OPTIONS_H
