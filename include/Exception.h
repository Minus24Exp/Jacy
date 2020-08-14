#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include "parser/Token.h"
#include "tree/Node.h"

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
 * @brief Common exception that can be used by any part of interpreter
 * @details This is mostly like system exceptions: FileNotFoundExceptions and so on
 * 
 * @param msg String message
 */
class CommonException : public YoctoException {
public:
    CommonException(const std::string & msg) : YoctoException(msg) {}
};

/**
 * FileNotFoundException
 */
class FileNotFoundException : public Common {
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
// Parsering Exceptions //
/////////////////////////

/**
 * UnexpectedTokenException
 */
class UnexpectedTokenException : public YoctoException {
public:
    UnexpectedTokenException(Token t) : YoctoException("Unexpected "+ t.to_string()) {}
};

/**
 * ExpectedException
 * @brief Unexpected end of file
 */
class UnexpectedEofException : public YoctoException {
public:
    UnexpectedEofException() : YoctoException("Unexpected end of file") {}
};

////////////////////////
// Runtime Exceptions //
////////////////////////
class RuntimeException : public YoctoException {
public:
    RuntimeException(const std::string & msg, const Position & pos, const std::string & in_file)
        : YoctoException("Runtime error: "+ msg +"\n"+ in_file +":"+ std::to_string(pos.line) +":"+ std::to_string(pos.column)) {}
};

class RecursionDepthExceeded : public YoctoException {
public:
    RecursionDepthExceeded(int limit) : YoctoException("Maximum recursion depth exceeded ("+ std::to_string(limit) +")") {}
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