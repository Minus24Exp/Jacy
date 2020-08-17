#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include "parser/Token.h"
#include "tree/Node.h"

const int RECURSION_DEPTH_LIMIT = 1000;

/**
 * @brief The base class of all exceptions in Yocto 
 * @details All other exceptions count as "Uncaught"
 * 
 * @param msg String message to print when caught
 */
class YoctoException : public std::exception {
public:
    YoctoException(const std::string & msg)
        : message(msg) {}

    /**
     * @return String message that exception was created with
     */
    virtual const char * what() const throw() {
        return message.c_str();
    }

private:
    const std::string message;
};

///////////////////////
// Common Exceptions //
///////////////////////

/**
 * FileNotFoundException
 */
class FileNotFoundException : public YoctoException {
public:
    FileNotFoundException() : YoctoException("File not found") {}
    FileNotFoundException(const std::string & details) : YoctoException("File not found: "+ details) {}
};

/**
 * IllegalArgumentException
 */
class IllegalArgumentException : public YoctoException {
public:
    IllegalArgumentException() : YoctoException("Illegal argument") {}
    IllegalArgumentException(const std::string & details) : YoctoException("Illegal argument: "+ details) {}
};

/**
 * ExpectedException
 * @brief Exception for cases when something was expected but wrong thing was given
 */
class ExpectedException : public YoctoException {
public:
    ExpectedException(const std::string & expected, const std::string & given)
        : YoctoException("Expected "+ expected +", "+ given +" given") {}

    ExpectedException(const std::string & expected, Token given_token)
        : ExpectedException(expected, given_token.to_string()) {}
};

/////////////////////////
// Parsring Exceptions //
/////////////////////////

class ParserException : public YoctoException {
public:
    ParserException(const std::string & msg) : YoctoException(msg) {}

    ParserException(const std::string & pre_msg, Token t, const std::string & post_msg)
        : ParserException(pre_msg +" "+ t.to_string() +" "+ post_msg) {}

    ParserException(const std::string & pre_msg, Token t)
        : ParserException(pre_msg, t, "") {}

    ParserException(Token t, const std::string & post_msg)
        : ParserException("", t, post_msg) {}
};

/**
 * UnexpectedTokenException
 */
class UnexpectedTokenException : public ParserException {
public:
    UnexpectedTokenException(Token t) : ParserException("Unexpected", t) {}
    UnexpectedTokenException(const std::string & token_str) : ParserException("Unexpected "+ token_str) {}
};

/**
 * ExpectedException
 * @brief Unexpected end of file
 */
class UnexpectedEofException : public ParserException {
public:
    UnexpectedEofException() : ParserException("Unexpected end of file") {}
};

////////////////////////
// Runtime Exceptions //
////////////////////////

/**
 * RuntimeException
 * 
 * @param msg String error message
 * @param pos Position where error occured
 * @param in_file File where error occured
 */
class RuntimeException : public YoctoException {
public:
    RuntimeException(const std::string & msg, const Position & pos, const std::string & in_file)
        : YoctoException("Runtime error: "+ msg +"\n"+ in_file +":"+
                        std::to_string(pos.line) +":"+ std::to_string(pos.column)) {}
};

/**
 * RecursionDepthLimit
 * @brief Error that occure when maximum recursion depth limit exceeded
 * 
 * @param pos Position where error occured
 * @param in_file File where error occured
 */
class RecursionDepthExceeded : public RuntimeException {
public:
    RecursionDepthExceeded(const Position & pos, const std::string & in_file)
        : RuntimeException("Maximum recursion depth exceeded ("+
                           std::to_string(RECURSION_DEPTH_LIMIT) +")", pos, in_file) {}
};

//////////////////////
// Other Exceptions //
//////////////////////

// Dev Error used in cases there something was wrong with source code
// It's mostly like a hint for developers
class DevError : public YoctoException {
public:
    DevError(const std::string & msg) : YoctoException("[Dev Error]: "+ msg) {}
};

#endif