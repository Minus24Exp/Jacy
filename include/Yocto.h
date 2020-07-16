#ifndef YOCTO_H
#define YOCTO_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <unordered_map>

#include "Exception.h"
#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "tree/Printer.h"
#include "backend/Interpreter.h"

class Yocto {
public:
    Yocto(int argc, const char * argv[]);
    virtual ~Yocto() = default;

    void launch();

    void run_prompt();
    void run_script(const std::string & path);

    void run(const std::string & script);
    void run_debug(const std::string & script);

private:
    std::string main_file;
    bool debug;

    Lexer & lexer;
    Parser & parser;
    Interpreter & ip;
};

#endif