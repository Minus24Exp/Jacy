#include "Jacy.h"

namespace jc {
    Jacy::Jacy() : log("", options.log) {}

    void Jacy::launch(int argc, const char ** argv) {
        config.config(argc, argv);

        options = config.jacy_options;
        log.options = config.jacy_options.log;
        lexer.options = config.lexer_options;
        lexer.log.options = config.lexer_options.log;
        parser.options = config.parser_options;
        parser.log.options = config.parser_options.log;
        compiler.options = config.compiler_options;
        compiler.log.options = config.compiler_options.log;
        lexer.options = config.lexer_options;
        lexer.log.options = config.lexer_options.log;
        lexer.options = config.lexer_options;
        lexer.log.options = config.lexer_options.log;

        // TODO: Add command line arguments module
        if (options.main_file.empty()) {
            run_repl();
        } else {
            run_script(options.main_file);
        }
    }

    void Jacy::run_repl() {
        main_file = "<REPL>";
        std::string line;
        while (!std::cin.eof()) {
            std::cout << "> ";

            line.clear();
            std::getline(std::cin, line);

            // TODO: !!! Fix problem with special keys like arrow (ConEmu)

            // Intercept exceptions for REPL
            // REPL just prints them and doesn't stop
            try {
                if (options.debug) {
                    run_debug(line);
                } else {
                    run(line);
                }
            } catch (JacyException & e) {
                log.error(e.what());
            }
        }
    }

    void Jacy::run_script(const std::string & path) {
        std::fstream file(path);

        if (!file.is_open()) {
            throw FileNotFoundException(path);
        }

        std::stringstream ss;
        ss << file.rdbuf();
        std::string script = ss.str();

        main_file = path;

        try {
            if (options.debug) {
                run_debug(script);
            } else {
                run(script);
            }
        } catch (JacyException & e) {
            log.error(e.what());
        }

        file.close();
    }

    void Jacy::run(const std::string & script) {
        parser::TokenStream tokens = lexer.lex(script, main_file);
        tree::StmtList tree = parser.parse(tokens);
        bytecode::Chunk chunk = compiler.compile(tree);
        vm.eval(chunk);
    }

    void Jacy::run_debug(const std::string & script) {
        // TODO: Create base exceptions for Lexer, Parser, Interpreter and catch them separately

        const auto bench = std::chrono::high_resolution_clock::now;

        // Lexing
        auto lexer_start = bench();
        parser::TokenStream tokens = lexer.lex(script, main_file);
        auto lexer_end = bench();

        std::cout << "Tokens:" << std::endl;
        for (auto & t : tokens) {
            std::cout << t.to_string(true) << std::endl;
        }

        if (options.run_level == common::RunLevel::Lexer) {
            log.info("Stop after lexing");
            return;
        }

        // Parse tokens
        auto parser_start = bench();
        tree::StmtList tree = parser.parse(tokens);
        auto parser_end = bench();

        if (options.run_level == common::RunLevel::Parser) {
            log.info("Stop after parsing");
            return;
        }

        // Print tree
        tree::Printer printer;
        std::cout << "\nParse Tree:" << std::endl;
        printer.print(tree);
        std::cout << std::endl;

        if (options.run_level == common::RunLevel::PrintTree) {
            log.info("Stop after tree printing");
            return;
        }

        auto compiler_start = bench();
        bytecode::Chunk chunk = compiler.compile(tree);
        auto compiler_end = bench();

        if (options.run_level == common::RunLevel::Compiler) {
            log.info("Stop after compiling");
            return;
        }

        disasm.eval(chunk);

        if (options.run_level == common::RunLevel::Disasm) {
            log.info("Stop after disassembling");
            return;
        }

        auto vm_start = bench();
        vm.eval(chunk);
        auto vm_end = bench();

        if (options.run_level == common::RunLevel::Vm) {
            log.info("Stop after evaluation");
            return;
        }

        std::cout << "\n\nBenchmarks:" << std::endl;

        auto lexer_duration = std::chrono::duration<double>(lexer_end - lexer_start).count();
        std::cout << "Lexing: " << lexer_duration << "s" << std::endl;

        auto parser_duration = std::chrono::duration<double>(parser_end - parser_start).count();
        std::cout << "Parsing: " << parser_duration << "s" << std::endl;

        auto compiler_duration = std::chrono::duration<double>(compiler_end - compiler_start).count();
        std::cout << "Compilation: " << compiler_duration << "s" << std::endl;

        auto vm_duration = std::chrono::duration<double>(vm_end - vm_start).count();
        std::cout << "Execution: " << vm_duration << "s" << std::endl;
    }
}