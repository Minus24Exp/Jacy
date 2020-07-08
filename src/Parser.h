#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>

#include "Token.h"
#include "tree/nodes.h"
#include "Exception.h"

class Parser {
public:
	static Parser & get_instance(){
		static Parser instance;
		return instance;
	}

    Parser(const Parser&) = delete;
    Parser(Parser&&) = delete;
    Parser & operator=(const Parser&) = delete;
    Parser & operator=(Parser&&) = delete;

private:
    Parser() = default;
    ~Parser() = default;

public:
	StmtList parse(const TokenStream & tokens);

private:
	TokenStream tokens;
	uint32_t index;

	StmtList tree;

	Token peek();
	Token advance();

	// Chekers
	bool eof();
	bool is_typeof(const TokenType & type);
	bool is_nl();
	bool is_semis();
	bool is_op(const Operator & op);
	bool is_kw(const Keyword & kw);

	bool is_infix_op();

	// Skippers
	void skip_nl(const bool & optional = false);
	void skip_semis();
	void skip_op(const Operator & op, const bool & skip_l_nl, const bool & skip_r_nl);
	void skip_kw(const Keyword & kw, const bool & skip_l_nl, const bool & skip_r_nl);

	// Parsers
	stmt_ptr parse_stmt();
	expr_ptr parse_expr();
	expr_ptr parse_atom();
	id_ptr parse_id();
	block_ptr parse_block(bool allow_one_line = false);
	expr_ptr parse_infix(expr_ptr left, int prec);
	stmt_ptr parse_var_decl();
	stmt_ptr parse_func_decl();
	expr_ptr parse_func_call(expr_ptr left);
	expr_ptr parse_if_expr();
	stmt_ptr parse_while();
	stmt_ptr parse_class_decl();

	// Errors
	void error(const std::string & msg);
	void unexpected_error();
	void expected_error(const std::string & expected);
};

#endif