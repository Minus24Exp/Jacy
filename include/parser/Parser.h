#ifndef PARSER_H
#define PARSER_H

#include "Exception.h"
#include "parser/Token.h"
#include "tree/nodes.h"

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

    // Virtual semi used when statement does not require semi after it, but parser need to know if semi was skipped
    bool virtual_semi = false;

    Token peek();
    Token advance();

    // Chekers //
    bool eof();
    bool is_typeof(const TokenType & type);
    bool is_nl();
    bool is_semis();
    bool is_op(const Operator & op);
    bool is_kw(const Keyword & kw);

    // Skippers //
    void skip_nl(const bool & optional = false);
    void skip_semis();
    void skip_op(const Operator & op, const bool & skip_l_nl, const bool & skip_r_nl);
    void skip_kw(const Keyword & kw, const bool & skip_l_nl, const bool & skip_r_nl);

    // Parsers //

    // Statements //
    stmt_ptr parse_stmt();
    block_ptr parse_block(bool allow_one_line = false);
    stmt_ptr parse_var_decl();
    stmt_ptr parse_func_decl();
    stmt_ptr parse_while_stmt();
    stmt_ptr parse_for_stmt();
    stmt_ptr parse_class_decl();
    stmt_ptr parse_import();

    // Expressions //
    expr_ptr parse_expr();

    // Precedence parsers (down-top) //
    expr_ptr assignment();
    expr_ptr pipe();
    expr_ptr Or();
    expr_ptr And();
    expr_ptr eq();
    expr_ptr comp();
    expr_ptr named_checks();
    expr_ptr range();
    expr_ptr add();
    expr_ptr mult();
    expr_ptr power();
    expr_ptr prefix();
    expr_ptr postfix();
    expr_ptr call();
    expr_ptr member_access();
    //

    expr_ptr primary();

    id_ptr parse_id();
    expr_ptr parse_func_call(expr_ptr left);
    expr_ptr parse_if_expr();

    // Errors
    void error(const std::string & msg);
    void unexpected_error();
    void expected_error(const std::string & expected);
};

#endif