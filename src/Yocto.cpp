#include "Yocto.h"

Yocto::Yocto(){}

void Yocto::run_prompt(){
	while(!std::cin.eof()){
		std::cout << "> ";
		std::string line;
		std::getline(std::cin, line);
		run(line);
		std::cout << std::endl;
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

	// TODO: Create base exceptions for Lexer, Parser, Interpreter
	// and catch them separatly

	const auto bench = std::chrono::high_resolution_clock::now;

	// Lexing
	Lexer lexer(script);
	auto lexer_start = bench();
	TokenStream tokens = lexer.lex();
	auto lexer_end = bench();

	// std::cout << "Tokens:" << std::endl;
	// for(auto & t : tokens){
	// 	std::cout << t.to_string() << std::endl;
	// }

	// Parse tokens
	Parser parser(tokens);
	auto parser_start = bench();
	StmtList tree = parser.parse();
	auto parser_end = bench();

	// Print tree
	// Printer printer;
	// std::cout << "\nParse Tree:" << std::endl;
	// printer.print(tree);
	// std::cout << std::endl;

	Interpreter interpreter;
	auto ip_start = bench();
	interpreter.interpret(tree);
	auto ip_end = bench();

	std::cout << "\n\nBenchmarks:" << std::endl;

	auto lexer_duration = std::chrono::duration<double>(lexer_end - lexer_start).count();
	std::cout << "Lexing: " << lexer_duration << "s" << std::endl;

	auto parser_duration = std::chrono::duration<double>(parser_end - parser_start).count();
	std::cout << "Parsing: " << parser_duration << "s" << std::endl;

	auto ip_duration = std::chrono::duration<double>(ip_end - ip_start).count();
	std::cout << "Evaluation: " << ip_duration << "s" << std::endl;
}