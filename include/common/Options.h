#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <cstdint>

namespace jc::common {
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
        LogLevel level{LogLevel::Warn};
    };

    // What parts to run
    enum RunLevel {
        Lexer,
        Parser,
        PrintTree,
        Compiler,
        Disasm,
        Vm,
    };

    // Jacy main options //
    struct JacyOptions {
        bool debug{false};
        RunLevel run_level{RunLevel::Vm}; // Note: For parser debug only
        std::string main_file;

        LoggerOptions log = {
            .log_class = false, // Jacy main class does not provide class name
            .level = LogLevel::Verbose, // Note: Verbose for debug
        };

        // Tests //
        bool run_tokens_test{false};
    };

    // Lexer //
    struct LexerOptions {
        LoggerOptions log = {};
    };

    // Parser //
    struct ParserOptions {
        bool log_parsing_entity{true};

        LoggerOptions log = {
            .level = LogLevel::Verbose, // Note: Verbose for debug
        };
    };

    // Printer //
    // struct PrinterOptions {};

    // Compiler //
    struct CompilerOptions {
        bool print_compiling_opcode{true};

        LoggerOptions log = {
            .level = LogLevel::Verbose, // Note: Verbose for debug
        };
    };

    // Disasm //
    struct DisasmOptions {
        bool pure_dump{true};

        LoggerOptions log = {};
    };

    // VM //
    struct VMOptions {
        // Note: ! Do not use logger inside vm logic, only for warnings and errors !
        LoggerOptions log = {};
    };

    // TODO: Think about Tree Printer options
}

#endif // OPTIONS_H
