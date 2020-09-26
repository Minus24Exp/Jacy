#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include <algorithm>
#include <vector>
#include <string>

using yo_int = long;

// Note: In Jacy floating-point numbers are doubles
enum class Operator;
enum class Keyword;
using TokenVal = std::variant<std::monostate, bool, yo_int, double, std::string, Operator, Keyword>;

struct Token;
using TokenStream = std::vector<Token>;

enum class TokenType {
    Null,
    Bool,
    Int,
    Float,
    String,
    Op,
    Kw,
    Id,
    Nl,
    Eof
};

// Number types for lexing
enum class NumType {
    Int,
    Float,
    Bin,
    Hex
};

enum class Operator {
    Assign,

    // Augmented assignment
    AddAssign, SubAssign, MulAssign, DivAssign, ModAssign, ExpAssign,

    Add, Sub, Mul, Div, Mod, Exp,

    // Punctuations
    LParen, RParen,
    LBrace, RBrace,
    LBracket, RBracket,

    Comma, Colon, Dot,

    Semi,

    Or, And,

    Not, Eq, NotEq,
    LT, GT, LE, GE,

    RefEq, RefNotEq,

    // RangeLE - Range left exclusive
    // RangeRE - Range right exclusive
    // RangeBothE - Range both exclusive
    Range, RangeLE, RangeRE, RangeBothE,

    Arrow,

    Is, NotIs,
    In, NotIn,

    As,

    Pipe
};

const std::vector <std::string> operators {
    "=",

    // Augmented assignment
    "+=", "-=", "*=", "/=", "%=", "**=",

    "+", "-", "*", "/", "%", "**",

    "(", ")",
    "{", "}",
    "[", "]",

    ",", ":", ".",

    ";",

    "||", "&&",

    "!", "==", "!=",
    "<", ">", "<=", ">=",

    "===", "!==",

    "..", ">..", "..<", ">.<",

    "=>",

    "is", "!is",
    "in", "!in",

    // Note: `as` is operator, but it used as keyword by `import` statement
    "as",

    "|>"
};

inline std::string op_to_str(const Operator & op) {
    return operators.at(static_cast<size_t>(op));
}

enum class Keyword {
    Null,
    True, False,
    Var, Val,

    Func,
    Return,

    If, Elif, Else,

    While,

    Class,

    Import,

    From,

    For,

    Type,

    MAX
};

// Overload operator less-than to check if identifier is Keyword
inline bool operator<(Keyword kwl, Keyword kwr) {
    return static_cast<size_t>(kwl) < static_cast<size_t>(kwr);
}

const std::vector <std::string> keywords {
    "null",
    "true", "false",
    "var", "val",

    "func",
    "return",

    "if", "elif", "else",

    "while",

    "class",

    "import",

    "from",

    "for",

    "type"
};

inline Keyword str_to_kw(const std::string & str) {
    return static_cast<Keyword>(
        std::distance(keywords.begin(), std::find(keywords.begin(), keywords.end(), str))
    );
}

inline std::string kw_to_str(const Keyword & kw) {
    return keywords.at(static_cast<size_t>(kw));
}

struct Position {
    uint32_t line = 0;
    uint32_t column = 0;
};

struct Token {
    TokenType type;
    TokenVal val;

    Position pos;

    Token(const TokenType & _type, const std::string & v) {
        this->type = _type;

        switch (type) {
            case TokenType::Id:
            case TokenType::String: {
                val = v;
            } break;
        }
    }

    Token(NumType num_type, const std::string & num) {
        switch (num_type) {
            case NumType::Int: {
                type = TokenType::Int;
                val = std::stol(num);
            } break;
            case NumType::Float: {
                type = TokenType::Float;
                val = std::stod(num);
            } break;
            case NumType::Bin: {
                type = TokenType::Int;
                val = std::stol(num, 0, 2);
            } break;
            case NumType::Hex: {
                type = TokenType::Int;
                val = std::stol(num, 0, 16);
            } break;
        }
    }

    Token(const Operator & op) {
        type = TokenType::Op;
        val = op;
    }

    Token(const Keyword & kw) {
        switch (kw) {
            case Keyword::True:
            case Keyword::False: {
                type = TokenType::Bool;
                val = kw == Keyword::True;
            } break;
            case Keyword::Null: {
                type = TokenType::Null;
            } break;
            default: {
                type = TokenType::Kw;
                val = kw;
            }
        }
    }

    Token(const TokenType & type) {
        this->type = type;
    }

    virtual ~Token() = default;

    bool Bool() {
        return std::get<bool>(val);
    }

    yo_int Int() {
        return std::get<yo_int>(val);
    }

    double Float() {
        return std::get<double>(val);
    }

    std::string String() {
        return std::get<std::string>(val);
    }

    Operator op() {
        return std::get<Operator>(val);
    }

    Keyword kw() {
        return std::get<Keyword>(val);
    }

    std::string to_string(bool with_pos = true) {
        std::string str;

        switch (type) {
            case TokenType::Null: {
                str += "null";
            } break;
            case TokenType::Bool: {
                str += "bool";
            } break;
            case TokenType::Int: {
                str += "int";
            } break;
            case TokenType::Float: {
                str += "float";
            } break;
            case TokenType::Id: {
                str += "identifier";
            } break;
            case TokenType::String: {
                str += "string";
            } break;
            case TokenType::Op: {
                str += "operator";
            } break;
            case TokenType::Kw: {
                str += "keyword";
            } break;
            case TokenType::Nl: {
                str += "[new line]";
            } break;
            case TokenType::Eof: {
                str += "[EOF]";
            } break;
        }

        // @TODO: Fix quote for empty values
        str += " `";
        switch (type) {
            case TokenType::Bool: {
                str += std::to_string(Bool());
            } break;
            case TokenType::Int: {
                str += std::to_string(Int());
            } break;
            case TokenType::Float: {
                str += std::to_string(Float());
            } break;
            case TokenType::Id:
            case TokenType::String: {
                str += String();
            } break;
            case TokenType::Op: {
                str += op_to_str(op());
            } break;
            case TokenType::Kw: {
                str += kw_to_str(kw());
            } break;
        }

        if (with_pos) {
            str += "` at "+ std::to_string(pos.line) +":"+ std::to_string(pos.column);
        }

        return str;
    }

    void error(const std::string & msg) {
        throw msg + " " + to_string();
    }
};

#endif