#include "Yocto.h"

Yocto::Yocto() : lexer(Lexer::get_instance()),
	   			 parser(Parser::get_instance()),
	   			 ip(Interpreter::get_instance()) {}

void Yocto::run_prompt(){
	std::string line;
	while(!std::cin.eof()){
		std::cout << "> ";

		line.clear();
		std::getline(std::cin, line);

		// TODO: Fix problem with special keys like arrow

		// Intercept exceptions for REPL
		// REPL just prints them and doesn't stop
		try{
			run(line);
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

	run(script);

	file.close();
}

void Yocto::run(const std::string & script){
	TokenStream tokens = lexer.lex(script);
	StmtList tree = parser.parse(tokens);
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