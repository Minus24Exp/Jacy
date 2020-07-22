#include "Yocto.h"

Yocto::Yocto(int argc, const char * argv[])
    : lexer(Lexer::get_instance()),
      parser(Parser::get_instance()),
      ip(Interpreter::get_instance())
{
    debug = false;

    // Parser argv
    for(int i = 1; i < argc; i++){
        std::string arg(argv[i]);

        if(arg[0] == '-'){
            if(arg.substr(1) == "debug"){
                debug = true;
            }
        }else{
            if(arg.find(".yo")){
                if(main_file.empty()){
                    main_file = arg;
                }else{
                    throw YoctoException("Expected only one input file");
                }
            }
        }
    }
}

void Yocto::launch(){
    if(main_file.empty()){
        run_repl();
    }else{
        run_script(main_file);
    }
}

void Yocto::run_repl(){
    std::string line;
    while(!std::cin.eof()){
        std::cout << "> ";

        line.clear();
        std::getline(std::cin, line);

        // TODO: !!! Fix problem with special keys like arrow (ConEmu)

        // Intercept exceptions for REPL
        // REPL just prints them and doesn't stop
        try{
            if(debug){
                run_debug(line);
            }else{
                run(line);
            }

            // TODO: Print last value only if there's no print function

            obj_ptr last_value = ip.get_value();
            if(last_value){
                std::cout << obj_to_str(last_value) << std::endl;
            }else{
                std::cout << "null" << std::endl;
            }

        }catch(YoctoException & e){
            std::cout << e.what() << std::endl;
        }
    }
}

void Yocto::run_script(const std::string & path){
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

void Yocto::run(const std::string & script){
    TokenStream tokens = lexer.lex(script);
    StmtList tree = parser.parse(tokens);

    Global global;
    global.reg();

    ip.interpret(tree);
}

void Yocto::run_debug(const std::string & script){

    // TODO: Create base exceptions for Lexer, Parser, Interpreter
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

    auto ip_start = bench();
    ip.interpret(tree);
    auto ip_end = bench();

    std::cout << "\n\nBenchmarks:" << std::endl;

    auto lexer_duration = std::chrono::duration<double>(lexer_end - lexer_start).count();
    std::cout << "Lexing: " << lexer_duration << "s" << std::endl;

    auto parser_duration = std::chrono::duration<double>(parser_end - parser_start).count();
    std::cout << "Parsing: " << parser_duration << "s" << std::endl;

    auto ip_duration = std::chrono::duration<double>(ip_end - ip_start).count();
    std::cout << "Evaluation: " << ip_duration << "s" << std::endl;
}