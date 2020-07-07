#ifndef YOCTO_H
#define YOCTO_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

#include "Exception.h"
#include "Lexer.h"
#include "Parser.h"
#include "Printer.h"
#include "backend/Interpreter.h"

class Yocto {
public:
	Yocto();
	virtual ~Yocto() = default;

	void run_prompt();
	void run_script(const std::string & path);

	void run(const std::string & script);
	
	void run_debug(const std::string & script);

private:
	Lexer & lexer;
	Parser & parser;
	Interpreter & ip;
};

#endif