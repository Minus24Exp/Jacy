#ifndef PARSER_H
#define PARSER_H

#include "Exception.h"
#include "parser/Token.h"
#include "tree/nodes.h"
#include "common/Logger.h"

namespace jc::parser {
    class Parser {
    public:
        Parser();
        virtual ~Parser() = default;

    public:
        tree::StmtList parse(const TokenStream & tokens);

    private:
        TokenStream tokens;
        uint32_t index;

        tree::StmtList tree;

        // Virtual semi used when statement does not require semi after it,
        // but parser need to know if semi was skipped
        bool virtual_semi = false;

        Token peek();
        Token advance();

        // Checkers //
        bool eof();
        bool is(const TokenType & type);
        bool is_after_nl(const TokenType & type, bool keep_nl = false);
        bool is_nl();
        bool is_semis();
        bool is_assign_op();
        bool is_literal();

        // Skippers //
        void skip_nl(bool optional = false);
        void skip_semis();
        void skip(const TokenType & type, bool skip_l_nl, bool skip_r_nl, const std::string & expected, bool _virtual_semi = false);
        bool opt_skip(const TokenType & type, bool skip_l_nl, bool skip_r_nl);

        // Parsers //

        // Statements //
        tree::stmt_ptr parse_stmt();
        tree::block_ptr parse_block(bool allow_one_line = false);
        tree::stmt_ptr parse_var_decl();
        tree::stmt_ptr parse_func_decl();
        tree::stmt_ptr parse_while_stmt();
        tree::stmt_ptr parse_for_stmt();
        tree::stmt_ptr parse_class_decl();
        tree::stmt_ptr parse_import();
        tree::stmt_ptr parse_type_decl();

        // Expressions //
        tree::expr_ptr parse_expr();

        // Precedence parsers (down-top) //
        tree::expr_ptr assignment();
        tree::expr_ptr pipe();
        tree::expr_ptr Or();
        tree::expr_ptr And();
        tree::expr_ptr bit_or();
        tree::expr_ptr Xor();
        tree::expr_ptr bit_and();
        tree::expr_ptr eq();
        tree::expr_ptr comp();
        tree::expr_ptr spaceship();
        tree::expr_ptr named_checks();
        tree::expr_ptr null_coalesce();
        tree::expr_ptr shift();
        tree::expr_ptr range();
        tree::expr_ptr add();
        tree::expr_ptr mult();
        tree::expr_ptr power();
        tree::expr_ptr type_cast();
        tree::expr_ptr prefix();
        tree::expr_ptr call();
        tree::expr_ptr member_access();
        tree::expr_ptr primary();
        //

        tree::id_ptr parse_id();
        tree::expr_ptr parse_func_call(const tree::expr_ptr & left);
        tree::expr_ptr parse_if_expr();
        tree::expr_ptr parse_literal();

        tree::type_ptr parse_type(const std::string & expected_type = "type");
        tree::type_ptr primary_type(const std::string & expected_type);

        // Errors //
        static void error(const std::string & msg, const Position & pos);
        void unexpected_error();
        void expected_error(const std::string & expected);

        // DEBUG //
    public:
        common::ParserOptions options;
        common::Logger log;

    private:
        void log_parsing_entity(const std::string & entity);
    };
}

#endif