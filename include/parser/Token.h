#ifndef TOKEN_H
#define TOKEN_H

#include <utility>
#include <variant>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

namespace jc::parser {
    struct Token;
    using TokenStream = std::vector<Token>;

    enum class TokenType {
        Int,
        Float,
        String,
        Id,
        Nl,
        Eof,

        // Operators and punctuations //

        // Operators
        Assign,
        AddAssign, SubAssign, MulAssign, DivAssign, ModAssign, ExpAssign,
        ShlAssign, ShrAssign, BitAndAssign, BitOrAssign, XorAssign, NullishAssign,
        Add, Sub, Mul, Div, Mod, Exp,
        Inc, Dec,
        Or, And, NullCoalesce,
        Shl, Shr,
        BitAnd, BitOr, Xor, BitNot,
        Not, Eq, NotEq,
        LT, GT, LE, GE, Cmp,
        RefEq, RefNotEq,
        Range, RangeLE, RangeRE, RangeBothE,
        Dot, SafeCall,
        Is, NotIs,
        In, NotIn,
        As, AsQM,

        // Punctuations
        Semi,
        DoubleArrow, Arrow,
        Pipe,
        LParen, RParen,
        LBrace, RBrace,
        LBracket, RBracket,
        Comma, Colon,
        Quest,

        // Keywords
        Null,
        True, False,
        Var, Val,
        Func,
        Return,
        If, Else,
        While,
        Class,
        Import,
        From,
        For,
        Type,

        None,
    };

    const auto op_start = static_cast<int>(TokenType::Assign);
    const std::vector <std::string> operators {
        // Operators
        "=",
        "+=", "-=", "*=", "/=", "%=", "**=",
        "<<=", ">>=", "&=", "|=", "^=", "??=",
        "+", "-", "*", "/", "%", "**",
        "++", "--",
        "||", "&&", "??",
        "&", "|", "^", "~",
        "<<", ">>",
        "!", "==", "!=",
        "<", ">", "<=", ">=", "<=>",
        "===", "!==",
        "..", ">..", "..<", ">.<",
        ".", "?.",
        "is", "!is",
        "in", "!in",
        "as", "as?",

        // Punctuations
        ";",
        "=>", "->",
        "|>",
        "(", ")",
        "{", "}",
        "[", "]",
        ",", ":",
    };

    inline std::string op_to_str(TokenType t) {
        return operators[static_cast<int>(t) - op_start];
    }

    const std::vector <std::string> keywords {
        "true", "false",
        "var", "val",
        "func",
        "return",
        "if", "else",
        "while",
        "class",
        "import",
        "from",
        "for",
        "type",
    };

    const auto kw_start = static_cast<int>(TokenType::True);
    inline TokenType str_to_kw(const std::string & str) {
        const auto & found = std::find(keywords.begin(), keywords.end(), str);
        if (found == keywords.end()) {
            return TokenType::None;
        }
        return static_cast<TokenType>(kw_start + std::distance(keywords.begin(), found));
    }

    inline std::string kw_to_str(TokenType t) {
        return keywords[static_cast<int>(t) - kw_start];
    }

    struct Position {
        uint32_t line = 0;
        uint32_t column = 0;
    };

    struct Token {
        TokenType type;
        std::string val;

        Position pos;

        Token(const TokenType & type, std::string val) : type(type), val(std::move(val)) {}

        std::string to_string(bool with_pos = false) const {
            std::string str;
            int index = static_cast<int>(type);
            switch (type) {
                case TokenType::Int:
                case TokenType::Float:
                case TokenType::String:
                case TokenType::Id: str += val; break;

                case TokenType::Nl: str += "[new-line]"; break;
                case TokenType::Eof: str += "[EOF]"; break;

                default: {
                    if (index >= op_start && index < kw_start) {
                        str += op_to_str(type);
                    } else if (index >= kw_start) {
                        str += kw_to_str(type);
                    } else {
                        str += "[unknown token " + std::to_string(index) + "]";
                    }
                }
            }
            if (with_pos) {
                str += " " + std::to_string(pos.line) +":"+ std::to_string(pos.column);
            }
            return str;
        }

        void error(const std::string & msg) {
            throw msg + " " + to_string(true);
        }
    };
}

#endif