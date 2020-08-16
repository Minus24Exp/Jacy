#ifndef TESTER_H
#define TESTER_H

#include "parser/Lexer.h"
#include "parser/Parser.h"
#include "tree/Printer.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>

// If true then after one test failed next tests won't be ran
const bool continue_after_fail = false;

class Tester {
public:
    /**
     * @return singleton instance of Tester class
     */
    static Tester & get_instance(){
        static Tester instance;
        return instance;
    }

/*
 * Delete everything singleton must not have
 */
public:
    Tester(const Tester&)             = delete;
    Tester(Tester&&)                  = delete;
    Tester & operator=(const Tester&) = delete;
    Tester & operator=(Tester&&)      = delete;

private:
    Tester();
    ~Tester() = default;

public:
    void run();

private:
    Lexer & lexer;
    Parser & parser;

    std::vector<std::string> test_list;
    void prepare();

    std::string read_file(const std::string & path);

    bool run_test(const std::string & path);
};

#endif