#include "parser/Lexer.h"

Lexer::Lexer() {
    index = 0;
    line = 0;
    column = 0;
    token_column = 0;
    token_line = 0;
}

char Lexer::peek() {
    return script[index];
}

char Lexer::peek_next(int distance) {
    if (index + distance > script.size()) {
        // If trying to peek to far it's unexpected eof
        unexpected_eof_error();
    }
    return script[index + distance];
}

char Lexer::advance(int inc) {
    for (int i = 0; i < inc; i++) {
        if (peek() == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        index++;
    }
    return peek();
}

bool Lexer::eof() {
    return peek() == '\0' || index >= script.size();
}

bool Lexer::skip(const char & c) {
    return c == '\t' || c == ' ' || c == '\r';
}

bool Lexer::is_nl(const char & c) {
    return c == '\n';
}

bool Lexer::is_digit(const char & c) {
    return c >= '0' && c <= '9';
}

bool Lexer::is_hex(const char & c) {
    return is_digit(c)
        || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

bool Lexer::is_id_first(const char & c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c == '_');
}

bool Lexer::is_id(const char & c) {
    return is_id_first(c) || is_digit(c);
}

bool Lexer::is_quote(const char & c) {
    return c == '"' || c == '\'' || c == '`';
}

void Lexer::add_token(Token t) {
    t.pos.line = token_line;
    t.pos.column = token_column;

    tokens.push_back(t);
}

void Lexer::add_token(const TokenType & type, const std::string & val) {
    add_token(Token(type, val));
}

void Lexer::add_token(const Operator & op) {
    add_token(Token(op));
}

void Lexer::add_token(const Keyword & kw) {
    add_token(Token(kw));
}

void Lexer::add_token(const TokenType & type) {
    add_token(Token(type));
}

void Lexer::add_token(NumType num_type, const std::string & num) {
    add_token(Token(num_type, num));
}

void Lexer::lex_number() {
    NumType num_type = NumType::Int;
    std::string num;

    if (peek() == '-') {
        num += '-';
        advance();
    }

    if (peek() == '0') {
        advance();
        switch (peek()) {
            case 'x':
            case 'X': {
                advance();
                if (!is_hex(peek())) {
                    unexpected_token_error();
                }
                do {
                    num += peek();
                } while(is_hex(advance()));

                add_token(NumType::Hex, num);
                return;
            } break;
            case 'b':
            case 'B': {
                advance();
                if (!is_digit(peek())) {
                    unexpected_token_error();
                }
                do {
                    num += peek();
                } while(is_digit(advance()));

                add_token(NumType::Bin, num);
                return;
            } break;
            default: num = "0";
        }
    }

    // @TODO: Fix floating hex numbers

    while (is_digit(peek())) {
        num += peek();
        advance();
    }

    if (peek() == '.') {
        // As far as numbers are object we must check if there's number after dot
        // and to advance through it
        if (!is_digit(peek_next())) {
            add_token(NumType::Int, num);
            return;
        }

        num_type = NumType::Float;
        num += peek();
        advance();
        if (!is_digit(peek())) {
            unexpected_token_error();
        }
        do {
            num += peek();
        } while(is_digit(advance()));
    }

    add_token(num_type, num);
}

TokenStream Lexer::lex(const std::string & script) {
    this->script = script;
    tokens.clear();
    index = 0;
    line = 1;
    column = 1;

    while (!eof()) {
        token_line = line;
        token_column = column;

        if (skip(peek())) {
            advance();
        } else if (is_nl(peek())) {
            add_token(TokenType::Nl);
            advance();
        } else if (is_digit(peek())) {
            lex_number();
        } else if (is_id_first(peek())) {
            std::string id(1, peek());
            while (is_id(advance())) {
                id += peek();
            }

            Keyword kw = str_to_kw(id);

            if (kw < Keyword::MAX) {
                // Note: !!! `elif` -> `else if` preprocessor
                if (kw == Keyword::Elif) {
                    add_token(Keyword::Else);
                    add_token(Keyword::If);
                } else {
                    add_token(kw);
                }
            } else if (id == "is") {
                // `is` operator
                add_token(Operator::Is);
            } else if (id == "in") {
                // `in` operator
                add_token(Operator::In);
            } else if (id == "as") {
                add_token(Operator::As);
            } else {
                add_token(TokenType::Id, id);
            }
        } else if (is_quote(peek())) {
            const char quote = peek();
            advance();
            std::string str = "";
            while (!eof()) {
                if (peek() == quote) {
                    break;
                }
                str += peek();
                advance();
            }
            if (eof()) {
                unexpected_eof_error();
            }
            if (peek() != quote) {
                unexpected_token_error();
            }
            add_token(TokenType::String, str);
            advance();
        } else {
            switch (peek()) {
                case '=': {
                    if (peek_next() == '>') {
                        add_token(Operator::Arrow);
                        advance(2);
                    } else if (peek_next() == '=') {
                        if (peek_next(2) == '=') {
                            add_token(Operator::RefEq);
                            advance(3);
                        } else {
                            add_token(Operator::Eq);
                            advance(2);
                        }
                    } else {
                        add_token(Operator::Assign);
                        advance();
                    }
                } break;
                case '+': {
                    if (peek_next() == '=') {
                        add_token(Operator::AddAssign);
                        advance(2);
                    } else {
                        add_token(Operator::Add);
                        advance();
                    }
                } break;
                case '-': {
                    if (is_digit(peek_next())) {
                        lex_number();
                    } else if (peek_next() == '=') {
                        add_token(Operator::SubAssign);
                        advance(2);
                    } else {
                        add_token(Operator::Sub);
                        advance();
                    }
                } break;
                case '*': {
                    if (peek_next() == '*') {
                        if (peek_next(2) == '=') {
                            add_token(Operator::ExpAssign);
                            advance(3);
                        } else {  
                            add_token(Operator::Exp);
                            advance(2);
                        }
                    } else if (peek_next() == '=') {
                        add_token(Operator::MulAssign);
                        advance(2);
                    } else {
                        add_token(Operator::Mul);
                        advance();
                    }
                } break;
                case '/': {
                    if (peek_next() == '/') {
                        while (!eof()) {
                            advance();
                            if (is_nl(peek())) {
                                break;
                            }
                        }
                    } else if (peek_next() == '*') {
                        while (!eof()) {
                            advance();
                            if (peek() == '*' && peek_next() == '/') {
                                break;
                            }
                        }
                        advance(2);
                    } else if (peek_next() == '=') {
                        add_token(Operator::DivAssign);
                        advance(2);
                    } else {
                        add_token(Operator::Div);
                        advance();
                    }
                } break;
                case '%': {
                    if (peek_next() == '=') {
                        add_token(Operator::ModAssign);
                        advance(2);
                    } else {
                        add_token(Operator::Mod);
                        advance();
                    }
                } break;
                case ';': {
                    add_token(Operator::Semi);
                    advance();
                } break;
                case '(': {
                    add_token(Operator::LParen);
                    advance();
                } break;
                case ')': {
                    add_token(Operator::RParen);
                    advance();
                } break;
                case '{': {
                    add_token(Operator::LBrace);
                    advance();
                } break;
                case '}': {
                    add_token(Operator::RBrace);
                    advance();
                } break;
                case '[': {
                    add_token(Operator::LBracket);
                    advance();
                } break;
                case ']': {
                    add_token(Operator::RBracket);
                    advance();
                } break;
                case ',': {
                    add_token(Operator::Comma);
                    advance();
                } break;
                case ':': {
                    add_token(Operator::Colon);
                    advance();
                } break;
                case '.': {
                    if (is_digit(peek_next())) {
                        lex_number();
                    } else if (peek_next() == '.') {
                        if (peek_next(2) == '.') {
                            add_token(Operator::Range);
                            advance(3);
                        } else if (peek_next(2) == '<') {
                            add_token(Operator::RangeRE);
                            advance(3);
                        } else {
                            unexpected_token_error();
                        }
                    } else {
                        add_token(Operator::Dot);
                        advance();
                    }
                } break;
                case '&': {
                    if (peek_next() == '&') {
                        add_token(Operator::And);
                        advance(2);
                    }
                } break;
                case '!': {
                    if (peek_next() == '=') {
                        if (peek_next(2) == '=') {
                            add_token(Operator::RefNotEq);
                            advance(3);
                        } else {
                            add_token(Operator::NotEq);
                            advance(2);
                        }
                    } else if (peek_next() == 'i' && peek_next(2) == 's') {
                        // `!is` operator
                        add_token(Operator::NotIs);
                        advance(3);
                    } else if (peek_next() == 'i' && peek_next(2) == 'n') {
                        add_token(Operator::NotIn);
                        advance(3);
                    } else {
                        add_token(Operator::Not);
                        advance();
                    }
                } break;
                case '|': {
                    if (peek_next() == '|') {
                        add_token(Operator::Or);
                        advance(2);
                    } else if (peek_next() == '>') {
                        add_token(Operator::Pipe);
                        advance(2);
                    } else {
                        unexpected_token_error();
                    }
                } break;
                case '<': {
                    if (peek_next() == '=') {
                        add_token(Operator::LE);
                        advance(2);
                    } else {
                        add_token(Operator::LT);
                        advance();
                    }
                } break;
                case '>': {
                    if (peek_next() == '=') {
                        add_token(Operator::GE);
                        advance(2);
                    } else if (peek_next() == '.') {
                        if (peek_next(2) == '.') {
                            add_token(Operator::RangeLE);
                            advance(3);
                        } else if (peek_next(2) == '<') {
                            add_token(Operator::RangeBothE);
                            advance(3);
                        } else {
                            unexpected_token_error();
                        }
                    } else {
                        add_token(Operator::GT);
                        advance();
                    }
                } break;
                default: {
                    unexpected_token_error();
                }
            }
        }
    }

    add_token(TokenType::Eof);

    return tokens;
}

void Lexer::unexpected_token_error() {
    std::string error = "token `"+ std::string(1, peek()) +"`";
    error += " at "+ std::to_string(line) +":"+ std::to_string(column);
    throw UnexpectedTokenException(error);
}

void Lexer::unexpected_eof_error() {
    throw UnexpectedEofException();
}
