#include "parser/Lexer.h"

namespace jc::parser {
    Lexer::Lexer() : log("Lexer", options.log) {
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

    void Lexer::add_token(const TokenType & type, const std::string & val = "") {
        add_token(Token(type, val));
    }

    void Lexer::lex_number() {
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

                    add_token(TokenType::Int, num);
                    return;
                }
                case 'b':
                case 'B': {
                    advance();
                    if (!is_digit(peek())) {
                        unexpected_token_error();
                    }
                    do {
                        num += peek();
                    } while(is_digit(advance()));

                    add_token(TokenType::Int, num);
                    return;
                }
                default: num = "0";
            }
        }

        // TODO: Fix floating hex numbers

        while (is_digit(peek())) {
            num += peek();
            advance();
        }

        if (peek() == '.') {
            // As far as numbers are object we must check if there's number after dot
            // and to advance through it
            if (!is_digit(peek_next())) {
                add_token(TokenType::Int, num);
                return;
            }

            num += peek();
            advance();
            if (!is_digit(peek())) {
                unexpected_token_error();
            }
            do {
                num += peek();
            } while(is_digit(advance()));
            add_token(TokenType::Float, num);
        } else {
            add_token(TokenType::Int, num);
        }
    }

    TokenStream Lexer::lex(const std::string & s) {
        this->script = s;
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

                if (id == "elif") {
                    add_token(TokenType::Else);
                    add_token(TokenType::If);
                    continue;
                }

                TokenType kw = str_to_kw(id);

                if (kw == TokenType::None) {
                    add_token(TokenType::Id, id);
                } else {
                    add_token(kw);
                }
            } else if (is_quote(peek())) {
                const char quote = peek();
                advance();
                std::string str;
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
                            add_token(TokenType::Arrow);
                            advance(2);
                        } else if (peek_next() == '=') {
                            if (peek_next(2) == '=') {
                                add_token(TokenType::RefEq);
                                advance(3);
                            } else {
                                add_token(TokenType::Eq);
                                advance(2);
                            }
                        } else {
                            add_token(TokenType::Assign);
                            advance();
                        }
                    } break;
                    case '+': {
                        if (peek_next() == '=') {
                            add_token(TokenType::AddAssign);
                            advance(2);
                        } else {
                            add_token(TokenType::Add);
                            advance();
                        }
                    } break;
                    case '-': {
                        if (is_digit(peek_next())) {
                            lex_number();
                        } else if (peek_next() == '=') {
                            add_token(TokenType::SubAssign);
                            advance(2);
                        } else {
                            add_token(TokenType::Sub);
                            advance();
                        }
                    } break;
                    case '*': {
                        if (peek_next() == '*') {
                            if (peek_next(2) == '=') {
                                add_token(TokenType::ExpAssign);
                                advance(3);
                            } else {
                                add_token(TokenType::Exp);
                                advance(2);
                            }
                        } else if (peek_next() == '=') {
                            add_token(TokenType::MulAssign);
                            advance(2);
                        } else {
                            add_token(TokenType::Mul);
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
                            add_token(TokenType::DivAssign);
                            advance(2);
                        } else {
                            add_token(TokenType::Div);
                            advance();
                        }
                    } break;
                    case '%': {
                        if (peek_next() == '=') {
                            add_token(TokenType::ModAssign);
                            advance(2);
                        } else {
                            add_token(TokenType::Mod);
                            advance();
                        }
                    } break;
                    case ';': {
                        add_token(TokenType::Semi);
                        advance();
                    } break;
                    case '(': {
                        add_token(TokenType::LParen);
                        advance();
                    } break;
                    case ')': {
                        add_token(TokenType::RParen);
                        advance();
                    } break;
                    case '{': {
                        add_token(TokenType::LBrace);
                        advance();
                    } break;
                    case '}': {
                        add_token(TokenType::RBrace);
                        advance();
                    } break;
                    case '[': {
                        add_token(TokenType::LBracket);
                        advance();
                    } break;
                    case ']': {
                        add_token(TokenType::RBracket);
                        advance();
                    } break;
                    case ',': {
                        add_token(TokenType::Comma);
                        advance();
                    } break;
                    case ':': {
                        add_token(TokenType::Colon);
                        advance();
                    } break;
                    case '.': {
                        if (is_digit(peek_next())) {
                            lex_number();
                        } else if (peek_next() == '.') {
                            if (peek_next(2) == '.') {
                                add_token(TokenType::Range);
                                advance(3);
                            } else if (peek_next(2) == '<') {
                                add_token(TokenType::RangeRE);
                                advance(3);
                            } else {
                                unexpected_token_error();
                            }
                        } else {
                            add_token(TokenType::Dot);
                            advance();
                        }
                    } break;
                    case '&': {
                        if (peek_next() == '&') {
                            add_token(TokenType::And);
                            advance(2);
                        }
                    } break;
                    case '!': {
                        if (peek_next() == '=') {
                            if (peek_next(2) == '=') {
                                add_token(TokenType::RefNotEq);
                                advance(3);
                            } else {
                                add_token(TokenType::NotEq);
                                advance(2);
                            }
                        } else if (peek_next() == 'i' && peek_next(2) == 's') {
                            // `!is` operator
                            add_token(TokenType::NotIs);
                            advance(3);
                        } else if (peek_next() == 'i' && peek_next(2) == 'n') {
                            add_token(TokenType::NotIn);
                            advance(3);
                        } else {
                            add_token(TokenType::Not);
                            advance();
                        }
                    } break;
                    case '|': {
                        if (peek_next() == '|') {
                            add_token(TokenType::Or);
                            advance(2);
                        } else if (peek_next() == '>') {
                            add_token(TokenType::Pipe);
                            advance(2);
                        } else {
                            unexpected_token_error();
                        }
                    } break;
                    case '<': {
                        if (peek_next() == '=') {
                            add_token(TokenType::LE);
                            advance(2);
                        } else {
                            add_token(TokenType::LT);
                            advance();
                        }
                    } break;
                    case '>': {
                        if (peek_next() == '=') {
                            add_token(TokenType::GE);
                            advance(2);
                        } else if (peek_next() == '.') {
                            if (peek_next(2) == '.') {
                                add_token(TokenType::RangeLE);
                                advance(3);
                            } else if (peek_next(2) == '<') {
                                add_token(TokenType::RangeBothE);
                                advance(3);
                            } else {
                                unexpected_token_error();
                            }
                        } else {
                            add_token(TokenType::GT);
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
}
