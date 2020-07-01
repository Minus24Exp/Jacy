#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>

#include "Token.h"
#include "tree/nodes.h"
#include "Exception.h"

class Parser {
public:
	Parser(const TokenStream & tokens);
	virtual ~Parser() = default;

	ParseTree parse();

private:
	TokenStream tokens;
	uint32_t index;

	ParseTree tree;

	Token peek();
	Token advance();

	// Chekers
	bool eof();
	bool is_typeof(const TokenType & type);
	bool is_nl();
	bool is_op(const Operator & op);
	bool is_kw(const Keyword & kw);

	bool is_infix_op();

	// Skippers
	void skip_nl(const bool & optional = false);
	void skip_semis();
	void skip_op(const Operator & op, const bool & skip_l_nl, const bool & skip_r_nl);
	void skip_kw(const Keyword & kw, const bool & skip_l_nl, const bool & skip_r_nl);

	// Parsers
	Statement * parse_statement();
	Expression * parse_expression();
	Expression * parse_atom();
	Identifier * parse_id();
	Block * parse_block();
	Expression * parse_infix(Expression * left, int prec);
	VarDecl * parse_var_decl();
	FuncDecl * parse_func_decl();
	FuncCall * parse_func_call(Expression * left);
	IfExpression * parse_if_expr();

	// Errors
	void error(const std::string & msg);
	void unexpected_error();
	void expected_error(const std::string & expected);
};

#endif