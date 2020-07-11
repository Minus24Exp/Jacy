#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include "Token.h"
#include "tree/Node.h"

class YoctoException : public std::exception {
public:
	YoctoException(const std::string & msg) : message(msg) {}

	virtual const char * what() const throw() {
		return message.c_str();
	}

private:
	const std::string message;
};

class FileNotFoundException : public YoctoException {
public:
	FileNotFoundException() : YoctoException("File not found") {}
	FileNotFoundException(const std::string & details) : YoctoException("File not found: "+ details) {}
};

class IllegalArgumentException : public YoctoException {
public:
	IllegalArgumentException() : YoctoException("Illegal argument") {}
	IllegalArgumentException(const std::string & details) : YoctoException("Illegal argument: "+ details) {}
};

class UnexpectedException : public YoctoException {
public:
	UnexpectedException(Token t) : YoctoException("Unexpected " + t.to_string()) {}
};

class ExpectedException : public YoctoException {
public:
	ExpectedException(const std::string & expected, const std::string & given)
		: YoctoException("Expected "+ expected +", "+ given +" given") {}

	ExpectedException(const std::string & expected, Token given_token)
		: ExpectedException(expected, given_token.to_string()) {}
};

class UnexpectedEofException : public YoctoException {
public:
	UnexpectedEofException() : YoctoException("Unexpected end of file") {}
};

class RuntimeException : public YoctoException {
public:
	RuntimeException(const std::string & msg, const Position & pos)
		: YoctoException("Runtime error: "+ msg +" at "+ std::to_string(pos.line) +":"+ std::to_string(pos.column)) {}
};

// Dev Error uses in cases there something was wrong with source code
class DevError : public YoctoException {
public:
	DevError(const std::string & msg) : YoctoException("[Dev Error]: "+ msg) {}
};

#endif