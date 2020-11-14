#ifndef LEXER_H
#define LEXER_H

#include "Exception.h"
#include "parser/Token.h"

#include <vector>
#include <iostream>

class Lexer {
public:
    Lexer();
    virtual ~Lexer() = default;

    TokenStream lex(const std::string & script);

private:
    std::string script;
    TokenStream tokens;

    void add_token(Token t);

    void add_token(const TokenType & type, const std::string & val);

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

    // Checkers //
    bool skip(const char & c);
    bool is_nl(const char & c);
    bool is_digit(const char & c);
    bool is_hex(const char & c);
    static bool is_id_first(const char & c);
    bool is_id(const char & c);
    static bool is_quote(const char & c);

    void lex_number();

    // Errors //
    void unexpected_token_error();
    static void unexpected_eof_error();
};

#endif