#include "Jacy.h"

Jacy::Jacy() : log("", options.log) {}

void Jacy::launch(int argc, const char * argv[]) {    
    // Parse argv

    // jacy_args -> false when main file appears
    bool jacy_args = true;

    std::vector<std::string> script_argv;
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);

        if (arg[0] == '-') {
            if (jacy_args) {
                // Parse Jacy arguments
                if (arg.substr(1) == "debug") {
                    options.debug = true;
                }
            } else {
                script_argv.emplace_back(argv[i]);
            }
        } else {
            // Check if argument is Jacy file (ends with ".jc")
            if (arg.size() > 3 && arg.compare(arg.size() - 3, 3, ".jc") == 0) {
                if (main_file.empty()) {
                    jacy_args = false;
                    main_file = arg;
                } else {
                    throw JacyException("Expected only one input file");
                }
            }
        }
    }

    if (main_file.empty()) {
        run_repl();
    } else {
        // @TODO: Add script argv available in script
        run_script(main_file);
    }
}

void Jacy::run_repl() {
    std::string line;
    while (!std::cin.eof()) {
        std::cout << "> ";

        line.clear();
        std::getline(std::cin, line);

        // @TODO: !!! Fix problem with special keys like arrow (ConEmu)

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
    TokenStream tokens = lexer.lex(script);
    StmtList tree = parser.parse(tokens);
    Chunk chunk = compiler.compile(tree);
    vm.eval(chunk);
}

void Jacy::run_debug(const std::string & script) {
    // @TODO: Create base exceptions for Lexer, Parser, Interpreter and catch them separatly

    const auto bench = std::chrono::high_resolution_clock::now;

    // Lexing
    auto lexer_start = bench();
    TokenStream tokens = lexer.lex(script);
    auto lexer_end = bench();

    std::cout << "Tokens:" << std::endl;
    for (auto & t : tokens) {
        std::cout << t.to_string(true) << std::endl;
    }

    // Parse tokens
    auto parser_start = bench();
    StmtList tree = parser.parse(tokens);
    auto parser_end = bench();

    // Print tree
    Printer printer;
    std::cout << "\nParse Tree:" << std::endl;
    printer.print(tree);
    std::cout << std::endl;
        
    auto compiler_start = bench();
    Chunk chunk = compiler.compile(tree);
    auto compiler_end = bench();

    disasm.eval(chunk);

    auto vm_start = bench();
    vm.eval(chunk);
    auto vm_end = bench();

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