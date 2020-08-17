#include "Jacy.h"

Jacy::Jacy()
    : lexer(Lexer::get_instance()),
      parser(Parser::get_instance())
{
    debug = false;
}

void Jacy::launch(int argc, const char * argv[]){    
    // Parse argv

    // Jacy_args -> false when main file appears
    bool Jacy_args = true;

    std::vector<std::string> script_argv;
    for(int i = 1; i < argc; i++){
        std::string arg(argv[i]);

        if(arg[0] == '-'){
            if(Jacy_args){
                // Parse Jacy arguments
                if(arg.substr(1) == "debug"){
                    debug = true;
                }
            }else{
                script_argv.push_back(std::string(argv[i]));
            }
        }else{
            // Check if argument is Jacy file (ends with ".yo")
            if(arg.size() > 3 && arg.compare(arg.size() - 3, 3, ".yo") == 0){
                if(main_file.empty()){
                    Jacy_args = false;
                    main_file = arg;
                }else{
                    throw JacyException("Expected only one input file");
                }
            }
        }
    }

    if(main_file.empty()){
        run_repl();
    }else{
        // @TODO: Add script argv available in script
        run_script(main_file);
    }
}

void Jacy::run_repl(){
    std::string line;
    while(!std::cin.eof()){
        std::cout << "> ";

        line.clear();
        std::getline(std::cin, line);

        // @TODO: !!! Fix problem with special keys like arrow (ConEmu)

        // Intercept exceptions for REPL
        // REPL just prints them and doesn't stop
        try{
            if(debug){
                run_debug(line);
            }else{
                run(line);
            }

        }catch(JacyException & e){
            std::cout << e.what() << std::endl;
        }
    }
}

void Jacy::run_script(const std::string & path){
    std::fstream file(path);

    if(!file.is_open()){
        throw FileNotFoundException(path);
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string script = ss.str();

    if(debug){
        run_debug(script);
    }else{
        run(script);
    }

    file.close();
}

void Jacy::run(const std::string & script){
    TokenStream tokens = lexer.lex(script);
    StmtList tree = parser.parse(tokens);
}

void Jacy::run_debug(const std::string & script){
    // @TODO: Create base exceptions for Lexer, Parser, Interpreter
    // and catch them separatly

    const auto bench = std::chrono::high_resolution_clock::now;

    // Lexing
    auto lexer_start = bench();
    TokenStream tokens = lexer.lex(script);
    auto lexer_end = bench();

    std::cout << "Tokens:" << std::endl;
    for(auto & t : tokens){
        std::cout << t.to_string() << std::endl;
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

    std::cout << "\n\nBenchmarks:" << std::endl;

    auto lexer_duration = std::chrono::duration<double>(lexer_end - lexer_start).count();
    std::cout << "Lexing: " << lexer_duration << "s" << std::endl;

    auto parser_duration = std::chrono::duration<double>(parser_end - parser_start).count();
    std::cout << "Parsing: " << parser_duration << "s" << std::endl;
}