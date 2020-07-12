#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

// Note: In Yocto floating-point numbers are doubles
enum class Operator;
enum class Keyword;
using TokenVal = std::variant<bool, int, double, std::string, Operator, Keyword>;

struct Token;
using TokenStream = std::vector<Token>;

enum class TokenType {
    Null,
    Bool,
    Int,
    Float,
    Str,
    Op,
    Kw,
    Id,
    Nl,
    Eof
};

enum class Operator {
    Assign,

    Add, Sub, Mul, Div, Mod,

    // Punctuations
    LParen, RParen,
    LBrace, RBrace,

    Comma, Colon, Dot,

    Semi,

    Or, And,

    Not, Eq, NotEq,
    LT, GT, LE, GE,

    Arrow
};

const std::vector <std::string> operators {
    "=",
    "+", "-", "*", "/", "%",

    "(", ")",
    "{", "}",

    ",", ":", ".",

    ";",

    "||", "&&",

    "!", "==", "!=",
    "<", ">", "<=", ">=",

    "=>"
};

inline std::string op_to_str(const Operator & op){
    return operators.at(static_cast<int>(op));
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
    
    MAX
};

// Overload operator less-than to check if identifier is Keyword
inline bool operator<(Keyword kwl, Keyword kwr){
    return static_cast<int>(kwl) < static_cast<int>(kwr);
}

const std::vector <std::string> keywords {
    "null",
    "true", "false",
    "var", "val",
    
    "func",
    "return",
    
    "if", "elif", "else",

    "while",

    "class"
};

inline Keyword str_to_kw(const std::string & str){
    return static_cast<Keyword>(
        std::distance(keywords.begin(), std::find(keywords.begin(), keywords.end(), str))
    );
}

inline std::string kw_to_str(const Keyword & kw){
    return keywords.at(static_cast<int>(kw));
}

struct Position {
    uint32_t line = 0;
    uint32_t column = 0;
};

struct Token {
    TokenType type;
    TokenVal val;

    Position pos;

    Token(const TokenType & _type, const std::string & v){
        this->type = _type;

        switch(type){
            case TokenType::Id:
            case TokenType::Str:{
                val = v;
                break;
            }
            case TokenType::Nl:{
                val = 0;
                break;
            }
            case TokenType::Eof:{
                val = 0;
                break;
            }
        }
    }

    Token(const int & i){
        type = TokenType::Int;
        val = i;
    }

    Token(const double & d){
        type = TokenType::Float;
        val = d;
    }

    Token(const Operator & op){
        type = TokenType::Op;
        val = op;
    }

    Token(const Keyword & kw){
        switch(kw){
            case Keyword::True:
            case Keyword::False:{
                type = TokenType::Bool;
                val = kw == Keyword::True;
                break;
            }
            case Keyword::Null:{
                type = TokenType::Null;
                val = 0;
                break;
            }
            default:{
                type = TokenType::Kw;
                val = kw;
            }
        }
    }

    Token(const TokenType & type){
        this->type = type;
        val = false;
    }

    virtual ~Token() = default;

    bool Bool(){
        return std::get<bool>(val);
    }

    int Int(){
        return std::get<int>(val);
    }

    double Float(){
        return std::get<double>(val);
    }

    std::string String(){
        return std::get<std::string>(val);
    }

    Operator op(){
        return std::get<Operator>(val);
    }
    
    Keyword kw(){
        return std::get<Keyword>(val);
    }

    std::string to_string(){
        std::string str;

        switch(type){
            case TokenType::Null:{
                str += "null";
                break;
            }
            case TokenType::Bool:{
                str += "bool";
                break;
            }
            case TokenType::Int:{
                str += "int";
                break;
            }
            case TokenType::Float:{
                str += "float";
                break;
            }
            case TokenType::Id:{
                str += "identifier";
                break;
            }
            case TokenType::Str:{
                str += "string";
                break;
            }
            case TokenType::Op:{
                str += "operator";
                break;
            }
            case TokenType::Kw:{
                str += "keyword";
                break;
            }
            case TokenType::Nl:{
                str += "[new line]";
                break;
            }
            case TokenType::Eof:{
                str += "[EOF]";
                break;
            }
        }

        // TODO: Fix quote for empty values
        str += " `";
        switch(type){
            case TokenType::Bool:{
                str += std::to_string(Bool());
                break;
            }
            case TokenType::Int:{
                str += std::to_string(Int());
                break;
            }
            case TokenType::Float:{
                str += std::to_string(Float());
                break;
            }
            case TokenType::Id:
            case TokenType::Str:{
                str += String();
                break;
            }
            case TokenType::Op:{
                str += op_to_str(op());
                break;
            }
            case TokenType::Kw:{
                str += kw_to_str(kw());
                break;
            }
        }

        str += "` at "+ std::to_string(pos.line) +":"+ std::to_string(pos.column);
        return str;
    }

    void error(const std::string & msg){
        throw msg + " " + to_string();
    }
};

#endif