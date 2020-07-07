#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>

#include "Token.h"
#include "Exception.h"

class Lexer {
public:
    static Lexer & get_instance(){
        static Lexer instance;
        return instance;
    }

    Lexer(const Lexer&) = delete;
    Lexer(Lexer&&) = delete;
    Lexer & operator=(const Lexer&) = delete;
    Lexer & operator=(Lexer&&) = delete;

private:
    Lexer() = default;
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
    void add_token(const int & i);
    void add_token(const double & d);

    uint32_t index;

    char peek();
    char peekNext();
    char advance();

    uint32_t line;
    uint32_t column;
    // Token column points to start of token
    uint32_t token_column;
    uint32_t token_line;

    bool eof();

    // Determinators
    bool skip(const char & c);
    bool is_nl(const char & c);
    bool is_digit(const char & c);
    bool is_hex(const char & c);
    bool is_id_first(const char & c);
    bool is_id(const char & c);
    bool is_quote(const char & c);

    void lex_number();

    // Errors
    void unexpected_error();
    void unexpected_eof_error();
};

#endif