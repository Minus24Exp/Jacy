#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>

#include "Exception.h"
#include "parser/Token.h"

class Lexer {
public:
    static Lexer & get_instance() {
        static Lexer instance;
        return instance;
    }

    Lexer(const Lexer&) = delete;
    Lexer(Lexer&&) = delete;
    Lexer & operator=(const Lexer&) = delete;
    Lexer & operator=(Lexer&&) = delete;

private:
    Lexer();
    ~Lexer() = default;

public:
    TokenStream lex(const std::string & script);

    std::string script;
    TokenStream tokens;

    void add_token(Token t);

    void add_token(const TokenType & type, const std::string & val);
    void add_token(const Operator & op);
    void add_token(const Keyword & kw);
    void add_token(const TokenType & type);
    void add_token(NumType num_type, const std::string & num);

    uint32_t index;
    uint32_t line;
    uint32_t column;
    // Token column points to start of token
    uint32_t token_column;
    uint32_t token_line;

    char peek();
    char peek_next(int distance = 1);
    char advance(int inc = 1);

    bool eof();

    // Checkers
    bool skip(const char & c);
    bool is_nl(const char & c);
    bool is_digit(const char & c);
    bool is_hex(const char & c);
    bool is_id_first(const char & c);
    bool is_id(const char & c);
    bool is_quote(const char & c);

    void lex_number();

    // Errors
    void unexpected_token_error();
    void unexpected_eof_error();
};

#endif