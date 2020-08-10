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

class Yocto {
public:
    static Yocto & get_instance(){
        static Yocto instance;
        return instance;
    }

    Yocto(const Yocto&) = delete;
    Yocto(Yocto&&) = delete;
    Yocto & operator=(const Yocto&) = delete;
    Yocto & operator=(Yocto&&) = delete;

private:
    Yocto();
    ~Yocto() = default;

public:
    void launch(int argc, const char * argv[]);

    void run_repl();
    void run_script(const std::string & path);

    void run(const std::string & script);
    void run_debug(const std::string & script);

private:
    std::string main_file;
    bool debug;

    Lexer & lexer;
    Parser & parser;
};

#endif
