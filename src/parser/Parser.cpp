#include "parser/Parser.h"

namespace jc::parser {
    Parser::Parser() : log("Parser", options.log), index(0) {}

    Token Parser::peek() {
        return tokens[index];
    }

    Token Parser::advance() {
        return tokens[++index];
    }

    /////////////
    // Checkers //
    /////////////
    bool Parser::eof() {
        return is(TokenType::Eof);
    }

    bool Parser::is(const TokenType & type) {
        return peek().type == type;
    }

    bool Parser::is_after_nl(const TokenType & type) {
        uint32_t old_index = index;
        skip_nl(true);
        if (is(type)) {
            return true;
        }
        index = old_index;
        return false;
    }

    bool Parser::is_nl() {
        return is(TokenType::Nl);
    }

    bool Parser::is_semis() {
        return is_nl() || is(TokenType::Semi);
    }

    bool Parser::is_assign_op() {
        // Fixme: Maybe reduce checkers?
        return is(TokenType::Assign)
            || is(TokenType::AddAssign)
            || is(TokenType::SubAssign)
            || is(TokenType::MulAssign)
            || is(TokenType::DivAssign)
            || is(TokenType::ModAssign)
            || is(TokenType::ExpAssign)
            || is(TokenType::ShlAssign)
            || is(TokenType::ShrAssign)
            || is(TokenType::BitAndAssign)
            || is(TokenType::BitOrAssign)
            || is(TokenType::XorAssign)
            || is(TokenType::NullishAssign);
    }

    bool Parser::is_literal() {
        return is(TokenType::True)
            || is(TokenType::False)
            || is(TokenType::Int)
            || is(TokenType::Float)
            || is(TokenType::String)
            || is(TokenType::Null);
    }

    //////////////
    // Skippers //
    //////////////
    void Parser::skip_nl(const bool & optional) {
        if (is_nl()) {
            do {
                advance();
            } while(is_nl());
        } else if (!optional) {
            expected_error("[new line]");
        }
    }

    void Parser::skip_semis() {
        if (is_semis()) {
            do {
                advance();
            } while(is_semis());
        } else {
            expected_error("';' or [new line] after statement");
        }
    }

    void Parser::skip(const TokenType & type, const bool & skip_l_nl, const bool & skip_r_nl, const std::string & expected) {
        if (skip_l_nl) {
            skip_nl(true);
        }
        if (is(type)) {
            advance();
        } else {
            // TODO: Pretty `given`
            expected_error(expected);
        }
        if (skip_r_nl) {
            skip_nl(true);
        }
    }

    bool Parser::opt_skip(const TokenType & type, const bool & skip_l_nl, const bool & skip_r_nl) {
        if (skip_l_nl && is_after_nl(type) || is(type)) {
            advance();
            if (skip_r_nl) {
                skip_nl(true);
            }
            return true;
        }
        return false;
    }

    /////////////
    // Parsers //
    /////////////
    tree::StmtList Parser::parse(const TokenStream & ts) {
        tree.clear();
        index = 0;
        this->tokens = ts;

        while (!eof()) {
            while (is_nl()) {
                advance();
            }
            if (eof()) {
                break;
            }

            tree.push_back(parse_stmt());

            if (!eof() && !virtual_semi) {
                skip_semis();
                virtual_semi = false;
            }
        }

        return tree;
    }

    ////////////////
    // Statements //
    ////////////////
    tree::stmt_ptr Parser::parse_stmt() {
        switch (peek().type) {
            case TokenType::Var:
            case TokenType::Val: {
                return parse_var_decl();
            }
            case TokenType::Func: {
                return parse_func_decl();
            }
            case TokenType::While: {
                return parse_while_stmt();
            }
            case TokenType::For: {
                return parse_for_stmt();
            }
            case TokenType::Return: {
                Position return_stmt_pos = peek().pos;
                advance();
                tree::expr_ptr expr = nullptr;
                if (!is_semis()) {
                    // not empty return
                    expr = parse_expr();
                }
                return std::make_shared<tree::ReturnStmt>(return_stmt_pos, expr);
            }
            case TokenType::Class: {
                return parse_class_decl();
            }
            case TokenType::Import: {
                return parse_import();
            }
            case TokenType::Type: {
                return parse_type_decl();
            }
            default: {
                return std::make_shared<tree::ExprStmt>(parse_expr());
            }
        }
    }

    // Block //
    tree::block_ptr Parser::parse_block(bool allow_one_line) {
        print_parsing_entity("block");

        Position block_pos = peek().pos;
        tree::StmtList stmts;

        // One-line //
        // If one-line block is allowed then try to parse single stmt
        if (!is(TokenType::LBrace) && allow_one_line) {
            // TODO: Think about this skip_nl
            //  Is it okay?
            skip_nl(true);
            stmts.push_back(parse_stmt());

            // Note: there's no need to expect semis after one-line block
            //  because block is always part of statement and after all statements
            //  there must be semi

            return std::make_shared<tree::Block>(block_pos, stmts);
        }

        // Multi-line //
        skip(TokenType::LBrace, false, true, "opening curly bracket '{' at block start");

        bool first = true;
        while (!eof()) {
            skip_nl(true);
            if (is(TokenType::RBrace)) {
                break;
            }
            if (first) {
                first = false;
            } else {
                skip_semis();
            }
            if (is(TokenType::RBrace)) {
                break;
            }
            stmts.push_back(parse_stmt());
        }

        skip(TokenType::RBrace, true, false, "closing curly bracket '}' at block end");

        virtual_semi = true;

        return std::make_shared<tree::Block>(block_pos, stmts);
    }

    // VarDecl //
    tree::stmt_ptr Parser::parse_var_decl() {
        print_parsing_entity("var_decl");

        Position var_decl_pos = peek().pos;

        tree::VarDeclKind decl = tree::VarDeclKind::Var;
        if (is(TokenType::Var)) {
            decl = tree::VarDeclKind::Var;
        } else if (is(TokenType::Val)) {
            decl = tree::VarDeclKind::Val;
        } else {
            expected_error("`var` or `val` keyword");
        }
        advance();

        tree::id_ptr id = parse_id();

        tree::type_ptr type = nullptr;
        if (opt_skip(TokenType::Colon, true, true)) {
            type = parse_type();
        }

        tree::expr_ptr assign_expr = nullptr;

        // It's obvious, but I'm marking that augmented assignment cannot appear in variable declaration
        if (opt_skip(TokenType::Assign, true, true)) {
            assign_expr = parse_expr();
        }

        return std::make_shared<tree::VarDecl>(var_decl_pos, decl, id, type, assign_expr);
    }

    // FuncDecl //
    tree::stmt_ptr Parser::parse_func_decl() {
        print_parsing_entity("func_decl");

        Position func_decl_pos = peek().pos;

        skip(TokenType::Func, false, true, "'func' keyword");

        tree::id_ptr id = parse_id();

        tree::FuncParams params;
        bool using_parens = opt_skip(TokenType::LParen, true, true);

        bool first = true;
        while (!eof()) {
            // For using_paren break on paren
            // For no-paren break on `=>` (block start) or `->` (type anno) or `{` (block start)
            // Note: This syntax is inference-capable because params parsing stops on `=>` or `{` but return type may be expected...
            if (using_parens && is(TokenType::RParen)
            || is(TokenType::DoubleArrow) || is(TokenType::Arrow) || is(TokenType::LBrace)) {
                break;
            }
            log.verbose("parse arg:", peek().to_string());
            if (first) {
                first = false;
            } else {
                skip(TokenType::Comma, true, true, "comma ',' to separate parameters");
            }

            bool vararg = false;
            if (is(TokenType::Spread)) {
                vararg = true;
                // TODO: Remove useless `skip`s
                skip(TokenType::Spread, false, false, "vararg operator '...'");
            }

            tree::id_ptr param_id = parse_id();

            // Find duplicates
            // TODO: Think about checking after all parameters added
            for (const auto & param : params) {
                if (param.id->get_name() == param_id->get_name()) {
                    error("Parameter duplication", param_id->pos);
                    return nullptr;
                }
            }

            // Note: Type annotation is required for parameters (with future inference too)
            skip(TokenType::Colon, true, true, "type annotation for parameter " + param_id->get_name());

            tree::type_ptr arg_type = parse_type();

            // Check for default value
            tree::expr_ptr default_val = nullptr;
            if (opt_skip(TokenType::Assign, true, true)) {
                default_val = parse_expr();
            }

            params.push_back({param_id, default_val, vararg, arg_type});
        }

        if (using_parens) {
            skip(TokenType::RParen, true, true, "closing parenthesis ')' after parameter list");

            // Note: Not inference-capable syntax
            if (is(TokenType::Arrow)) {
                skip(TokenType::Arrow, false, true, "'->' for return type annotation");
            } else if (is(TokenType::Colon)) {
                skip(TokenType::Colon, false, true, "':' for return type annotation");
            } else {
                expected_error("'->' or ':' for return type annotation");
            }
        } else {
            // For no-paren syntax only `->` anno is available
            skip(TokenType::Arrow, true, true, "'->' for return type annotation");
        }

        // Parse type after `:` or `->`
        tree::type_ptr return_type = parse_type();

        bool allow_one_line = false;
        if (opt_skip(TokenType::DoubleArrow, true, true)) {
            allow_one_line = true;
        }

        tree::block_ptr body = parse_block(allow_one_line);

        return std::make_shared<tree::FuncDecl>(func_decl_pos, id, params, body, return_type);
    }

    // WhileStmt //
    tree::stmt_ptr Parser::parse_while_stmt() {
        print_parsing_entity("while_stmt");

        Position while_pos = peek().pos;

        skip(TokenType::While, false, true, "'while' keyword");

        tree::expr_ptr cond = parse_expr();

        bool allow_one_line = false;
        if (is_nl()) {
            allow_one_line = true;
        }

        if (opt_skip(TokenType::DoubleArrow, true, true)) {
            allow_one_line = true;
        }

        tree::block_ptr body = parse_block(allow_one_line);

        return std::make_shared<tree::WhileStmt>(while_pos, cond, body);
    }

    // ForStmt //
    tree::stmt_ptr Parser::parse_for_stmt() {
        print_parsing_entity("for_stmt");

        Position for_stmt_pos = peek().pos;

        skip(TokenType::For, false, true, "'for' keyword");

        tree::id_ptr For = parse_id();

        skip(TokenType::In, true, true, "'in' keyword");

        tree::expr_ptr In = parse_expr();

        bool allow_one_line = false;
        if (is_nl()) {
            allow_one_line = true;
        }

        if (opt_skip(TokenType::DoubleArrow, true, true)) {
            allow_one_line = true;
        }

        tree::block_ptr body = parse_block(allow_one_line);

        return std::make_shared<tree::ForStmt>(for_stmt_pos, For, In, body);
    }

    // ClassDecl //
    tree::stmt_ptr Parser::parse_class_decl() {
        print_parsing_entity("class_decl");

        Position class_decl_pos = peek().pos;

        skip(TokenType::Class, false, true, "'class' keyword");

        tree::id_ptr id = parse_id();

        skip_nl(true);

        tree::expr_ptr super = nullptr;
        if (opt_skip(TokenType::Colon, true, true)) {
            super = parse_expr();
        }

        skip(TokenType::LBrace, true, true, "opening curly bracket '{' at start of class body");

        // TODO!: Think about nested classes

        // Parse declarations
        tree::StmtList decls;
        while (!eof()) {
            skip_nl(true);

            if (is(TokenType::RBrace)) {
                break;
            }

            if (is(TokenType::Val) || is(TokenType::Var)) {
                decls.push_back(parse_var_decl());
            } else if (is(TokenType::Func)) {
                decls.push_back(parse_func_decl());
            } else {
                expected_error("function or variable declaration");
            }
            skip_semis();
        }

        skip(TokenType::RBrace, true, false, "closing curly bracket '}' at end of class body");

        return std::make_shared<tree::ClassDecl>(class_decl_pos, id, super, decls);
    }

    // Import //
    tree::stmt_ptr Parser::parse_import() {
        print_parsing_entity("import");

        Position import_pos = peek().pos;
        // No new-lines in import
        skip(TokenType::Import, false, true, "'import' keyword");

        // TODO: Improve `import`
        // - Multiple objects import

        // Import nothing, just run source
        // e.g. `import "path"`
        if (is(TokenType::String)) {
            std::string path = peek().val;
            advance();
            return std::make_shared<tree::Import>(import_pos, path);
        }

        // Complex import
        tree::ImportEntityList entities;
        bool first = true;
        while (!eof()) {
            skip_nl(true);
            if (is_nl() || is(TokenType::From)) {
                break;
            }
            if (first) {
                first = false;
            } else {
                skip(TokenType::Comma, false, false, "',' to separate import entities");
            }
            if (is_nl() || is(TokenType::From)) {
                break;
            }

            bool all = false;
            tree::id_ptr object = nullptr;
            tree::id_ptr as = nullptr;
            if (is(TokenType::Mul)) {
                advance();
                all = true;
            } else {
                object = parse_id();
            }

            // All (`*`) requires `as` annotation
            if (all || is(TokenType::As)) {
                skip(TokenType::As, false, false, "all '*' selection or 'as' selection");
                as = parse_id();
            } else {
                as = nullptr;
            }

            entities.push_back({ all, object, as });
        }

        skip(TokenType::From, true, true, "'from' keyword to specify importing file");

        if (!is(TokenType::String)) {
            expected_error("path to file (String)");
        }

        std::string path = peek().val;
        advance();

        return std::make_shared<tree::Import>(import_pos, path, entities);
    }

    // TypeDecl //
    tree::stmt_ptr Parser::parse_type_decl() {
        print_parsing_entity("type_decl");

        Position type_decl_pos = peek().pos;

        skip(TokenType::Type, false, true, "'type' keyword");
        tree::id_ptr id = parse_id();
        skip(TokenType::Assign, true, true, "'=' operator. Type aliases require immediate initialization");
        tree::type_ptr type = parse_type();

        return std::make_shared<tree::TypeDecl>(type_decl_pos, id, type);
    }

    /////////////////
    // Expressions //
    /////////////////
    tree::expr_ptr Parser::parse_expr() {
        return assignment();
    }

    tree::expr_ptr Parser::assignment() {
        tree::expr_ptr expr = pipe();

        // TODO: Add compound assignment operators

        if (is_assign_op()) {
            print_parsing_entity("assignment");

            Token assign_op = peek();
            advance();

            // Fixme: skip_nl?

            tree::expr_ptr value = parse_expr();

            if (expr->type == tree::ExprType::Id) {
                tree::id_ptr id = std::static_pointer_cast<tree::Identifier>(expr);
                return std::make_shared<tree::Assign>(id, value, assign_op);
            }

            if (expr->type == tree::ExprType::Get) {
                std::shared_ptr<tree::GetExpr> get_expr = std::static_pointer_cast<tree::GetExpr>(expr);
                return std::make_shared<tree::SetExpr>(get_expr->left, get_expr->id, assign_op, value);
            }

            if (expr->type == tree::ExprType::GetItem) {
                std::shared_ptr<tree::GetItem> get_item = std::static_pointer_cast<tree::GetItem>(expr);
                return std::make_shared<tree::SetItem>(get_item->left, get_item->index, assign_op, value);
            }

            unexpected_error();
        }

        return expr;
    }

    tree::expr_ptr Parser::pipe() {
        tree::expr_ptr left = Or();

        while (is(TokenType::Pipe)) {
            print_parsing_entity("pipe");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = Or();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::Or() {
        tree::expr_ptr left = And();

        while (is(TokenType::Or)) {
            print_parsing_entity("or");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = And();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::And() {
        tree::expr_ptr left = bit_or();

        while (is(TokenType::And)) {
            print_parsing_entity("and");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = bit_or();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::bit_or() {
        tree::expr_ptr left = Xor();

        while (is(TokenType::BitOr)) {
            print_parsing_entity("bit_or");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = Xor();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::Xor() {
        tree::expr_ptr left = bit_and();

        while (is(TokenType::BitAnd)) {
            print_parsing_entity("xor");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = bit_and();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::bit_and() {
        tree::expr_ptr left = eq();

        while (is(TokenType::BitAnd)) {
            print_parsing_entity("bit_and");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = eq();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::eq() {
        tree::expr_ptr left = comp();

        while (is(TokenType::Eq) || is(TokenType::NotEq)
            || is(TokenType::RefEq) || is(TokenType::RefNotEq)) {
            print_parsing_entity("eq");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = comp();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::comp() {
        tree::expr_ptr left = spaceship();

        while (is(TokenType::LT)
            || is(TokenType::GT)
            || is(TokenType::LE)
            || is(TokenType::GE)) {
            print_parsing_entity("comp");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = spaceship();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::spaceship() {
        tree::expr_ptr left = named_checks();

        while (is(TokenType::Cmp)) {
            print_parsing_entity("spaceship");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = named_checks();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::named_checks() {
        tree::expr_ptr left = null_coalesce();

        while (is(TokenType::Is)
            || is(TokenType::NotIs)
            || is(TokenType::In)
            || is(TokenType::NotIn)) {
            print_parsing_entity("named_checks");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = null_coalesce();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::null_coalesce() {
        tree::expr_ptr left = shift();

        while (is(TokenType::NullCoalesce)) {
            print_parsing_entity("null_coalesce");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = shift();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::shift() {
        tree::expr_ptr left = range();

        while (is(TokenType::Shr) || is(TokenType::Shl)) {
            print_parsing_entity("shift");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = range();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::range() {
        tree::expr_ptr left = add();

        // TODO: Think if range to range is possible, now parse only `a..b` not `a..b..c`
        if (is(TokenType::Range)
         || is(TokenType::RangeLE)
         || is(TokenType::RangeRE)
         || is(TokenType::RangeBothE)) {
            print_parsing_entity("range");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = add();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::add() {
        tree::expr_ptr left = mult();

        while (is(TokenType::Add) || is(TokenType::Sub)) {
            print_parsing_entity("add");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = mult();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::mult() {
        tree::expr_ptr left = power();

        while (is(TokenType::Mul) || is(TokenType::Div) || is(TokenType::Mod)) {
            print_parsing_entity("mult");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = power();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::power() {
        tree::expr_ptr left = type_cast();

        while (is(TokenType::Exp)) {
            print_parsing_entity("power");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = type_cast();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::type_cast() {
        tree::expr_ptr left = prefix();

        if (is(TokenType::As) || is(TokenType::AsQM)) {
            print_parsing_entity("type_cast");

            const auto & op_token = peek();
            advance();
            skip_nl(true);
            tree::expr_ptr right = prefix();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::prefix() {
        if (is(TokenType::Not) || is(TokenType::Sub)) {
            print_parsing_entity("prefix");

            const auto & op_token = peek();
            advance();
            tree::expr_ptr right = call();
            return std::make_shared<tree::Prefix>(op_token, right);
        }

        return call();
    }

    tree::expr_ptr Parser::call() {
        tree::expr_ptr left = member_access();

        while (!eof()) {
            if (is(TokenType::LParen)) {
                left = parse_func_call(left);
            } else {
                break;
            }
        }

        return left;
    }

    tree::expr_ptr Parser::member_access() {
        tree::expr_ptr left = primary();

        while (!eof()) {
            if (is(TokenType::Dot)) {
                print_parsing_entity("get_expr");

                advance();
                tree::id_ptr id = parse_id();

                if (is(TokenType::LParen)) {
                    // We put left as nullptr, cause we already know left expression
                    const auto & func_call = std::static_pointer_cast<tree::FuncCall>(parse_func_call(nullptr));
                    left = std::make_shared<tree::MethodCall>(left, id, func_call->args);
                } else {
                    left = std::make_shared<tree::GetExpr>(left, id);
                }
            } else if (is(TokenType::LBracket)) {
                print_parsing_entity("sub-expression");

                skip(TokenType::LBracket, false, true, "opening bracket '[' for index access");
                tree::expr_ptr ind = parse_expr();
                skip(TokenType::RBracket, true, false, "closing bracket ']' after index access");
                left = std::make_shared<tree::GetItem>(left, ind);
            } else {
                break;
            }
        }

        return left;
    }

    tree::expr_ptr Parser::primary() {
        // Literal //
        if (is_literal()) {
            return parse_literal();
        }

        // Id //
        if (is(TokenType::Id)) {
            return parse_id();
        }

        // Grouping //
        if (opt_skip(TokenType::LParen, false, true)) {
            print_parsing_entity("grouping");

            tree::expr_ptr expr = parse_expr();

            skip(TokenType::RParen, true, false, "closing parenthesis ')' at end of grouping");

            // TODO: !!! Think do I need special node for grouping? (precedence problem?)
            return expr;
        }

        Position primary_pos = peek().pos;

        // IfExpr //
        if (is(TokenType::If)) {
            return parse_if_expr();
        }

        // ListExpr //
        if (opt_skip(TokenType::LBracket, false, true)) {
            print_parsing_entity("list");

            tree::ExprList elements;
            bool first = true;
            while (!eof()) {
                skip_nl(true);
                if (is(TokenType::RBracket)) {
                    break;
                }
                if (first) {
                    first = false;
                } else {
                    skip(TokenType::Comma, true, true, "comma ',' to separate list elements");
                }
                // Note: Allow `[1,]` (comma without next element)
                if (is(TokenType::RBracket)) {
                    break;
                }
                // ...element
                const auto & pos = peek().pos;
                if (opt_skip(TokenType::Spread, true, true)){
                    const auto & expr = parse_expr();
                    elements.push_back(std::make_shared<tree::SpreadExpr>(pos, expr));
                } else {
                    elements.push_back(parse_expr());
                }
            }
            skip(TokenType::RBracket, true, false, "closing bracket ']' after list literal");
            return std::make_shared<tree::ListExpr>(primary_pos, elements);
        }

        // Dictionary
        if (opt_skip(TokenType::LBrace, false, true)) {
            print_parsing_entity("dict");

            tree::DictElementList elements;
            bool first = true;
            while (!eof()) {
                skip_nl(true);
                if (is(TokenType::RBrace)) {
                    break;
                }
                if (first) {
                    first = false;
                } else {
                    skip(TokenType::Comma, true, true, "comma ',' to separate dictionary elements");
                }
                if (is(TokenType::RBrace)) {
                    break;
                }

                tree::id_ptr id_key = nullptr;
                tree::expr_ptr expr_key = nullptr;

                if (opt_skip(TokenType::LBracket, true, true)) {
                    expr_key = parse_expr();
                    skip(TokenType::RBracket, true, true, "closing bracket ']' after expression key");
                } else if (is_literal()) {
                    expr_key = parse_literal();
                } else if (is(TokenType::Id)) {
                    id_key = parse_id();
                } else {
                    expected_error("key: [expression], literal or identifier");
                }

                skip(TokenType::Colon, true, true, "colon ':' to separate key and value in dictionary");

                if (is(TokenType::Spread)) {
                    const auto & pos = peek().pos;
                    const auto & val = parse_expr();
                    elements.push_back({id_key, expr_key, std::make_shared<tree::SpreadExpr>(pos, val)});
                } else {
                    const auto & val = parse_expr();
                    elements.push_back({id_key, expr_key, val});
                }
            }
            skip(TokenType::RBrace, true, false, "closing curly bracket '}' after dictionary literal");
            return std::make_shared<tree::DictExpr>(primary_pos, elements);
        }

        expected_error("primary expression");

        return nullptr;
    }

    // Identifier //
    tree::id_ptr Parser::parse_id() {
        print_parsing_entity("id");

        if (!is(TokenType::Id)) {
            expected_error("identifier");
        }

        tree::id_ptr id = std::make_shared<tree::Identifier>(peek());
        advance();
        return id;
    }

    // FuncCall //
    tree::expr_ptr Parser::parse_func_call(const tree::expr_ptr & left) {
        print_parsing_entity("func_call");

        skip(TokenType::LParen, false, true, "[DEV_ERROR] opening parenthesis '(' in function call");

        tree::FuncArgs args;

        bool first = true;
        while (!eof()) {
            skip_nl(true);
            if (is(TokenType::RParen)) {
                break;
            }
            if (first) {
                first = false;
            } else {
                skip(TokenType::Comma, true, true, "comma ',' to separate arguments in function call");
            }
            if (is(TokenType::RParen)) {
                break;
            }
            bool spread = false;
            if (opt_skip(TokenType::Spread, false, true)) {
                spread = true;
            }
            args.push_back({parse_expr(), spread});
        }

        skip(TokenType::RParen, true, false, "closing parenthesis ')'");

        return std::make_shared<tree::FuncCall>(left, args);
    }

    // IfExpr //
    tree::expr_ptr Parser::parse_if_expr() {
        print_parsing_entity("if_expr");

        Position if_pos = peek().pos;

        skip(TokenType::If, false, true, "'if' keyword");

        tree::expr_ptr cond = parse_expr();

        bool allow_one_line = false;
        if (is_nl()) {
            // If `if` condition is not captured in parenthesis,
            // then only if there's new-line after it, body can be one-line
            allow_one_line = true;
        }

        if (opt_skip(TokenType::DoubleArrow, true, true)) {
            allow_one_line = true;
        }

        tree::block_ptr then_branch = parse_block(allow_one_line);

        // Allow to write one-line expressions
        // like: val a = if true => 'yeps' else 'nope'
        if (!is(TokenType::Else) && !virtual_semi) {
            skip_semis();
            virtual_semi = true;
        }

        tree::block_ptr else_branch = nullptr;
        if (opt_skip(TokenType::Else, false, true)) {
            else_branch = parse_block(true);
        }

        return std::make_shared<tree::IfExpr>(if_pos, cond, then_branch, else_branch);
    }

    tree::expr_ptr Parser::parse_literal() {
        print_parsing_entity("literal");

        Token current = peek();
        advance();
        return std::make_shared<tree::Literal>(current);
    }

    ///////////
    // Types //
    ///////////
    tree::type_ptr Parser::parse_type() {
        const auto & pos = peek().pos;
        tree::type_ptr left;
        if (is(TokenType::Id)) {
            print_parsing_entity("id_type");

            tree::id_type_ptr id_type = std::make_shared<tree::IdType>(pos, parse_id());

            if (opt_skip(TokenType::LT, false, true)) {
                // GenericType //

                std::vector<tree::type_ptr> types;
                bool first = true;
                while (!eof() || !is(TokenType::GT)) {
                    skip_nl(true);
                    if (first) {
                        first = false;
                    } else {
                        skip(TokenType::Comma, true, true, "comma ',' to separate generic type arguments");
                    }
                    types.push_back(parse_type());
                    skip_nl(true);
                }
                skip(TokenType::GT, true, false, "'>' to end generic type arguments");
                left = std::make_shared<tree::GenericType>(id_type, types);
            } else {
                left = id_type;
            }
        } else if (opt_skip(TokenType::LBracket, false, true)) {
            print_parsing_entity("list_type");

            left = std::make_shared<tree::ListType>(pos, parse_type());
            skip(TokenType::RBracket, true, false, "closing bracket ']' at end of list type");
        } else if (opt_skip(TokenType::LBrace, true, true)) {
            print_parsing_entity("dict_type");

            const auto & key = parse_type();
            skip(TokenType::Colon, true, true, "colon ':' to separate key and value types in dictionary type");
            const auto & val = parse_type();
            left = std::make_shared<tree::DictType>(pos, key, val);

            skip(TokenType::RBrace, true, false, "closing curly bracket '}' at end of dictionary type");
        } else {
            expected_error("type");
        }

        if (is(TokenType::BitOr)) {
            print_parsing_entity("union_type");

            return std::make_shared<tree::UnionType>(left, parse_type());
        }

        return left;
    }

    ////////////
    // Errors //
    ////////////
    void Parser::error(const std::string & msg, const Position & pos) {
        throw ParserException(msg, pos);
    }

    void Parser::unexpected_error() {
        throw UnexpectedTokenException(peek());
    }

    void Parser::expected_error(const std::string & expected) {
        throw ExpectedException(expected, peek());
    }

    ///////////
    // Debug //
    ///////////
    void Parser::print_parsing_entity(const std::string & entity) {
        if (!options.log_parsing_entity) {
            return;
        }
        log.debug("Parse", entity);
    }
}
