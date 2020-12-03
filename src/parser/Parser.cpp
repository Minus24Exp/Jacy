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

    bool Parser::is_after_nl(const TokenType & type, bool keep_nl) {
        uint32_t old_index = index;
        skip_nl(true);
        if (is(type)) {
            if (keep_nl) {
                index = old_index;
            }
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
        // Note!: Automatically skips optional end-lines
        return opt_skip(TokenType::Assign, true, true)
            || opt_skip(TokenType::AddAssign, true, true)
            || opt_skip(TokenType::SubAssign, true, true)
            || opt_skip(TokenType::MulAssign, true, true)
            || opt_skip(TokenType::DivAssign, true, true)
            || opt_skip(TokenType::ModAssign, true, true)
            || opt_skip(TokenType::ExpAssign, true, true)
            || opt_skip(TokenType::ShlAssign, true, true)
            || opt_skip(TokenType::ShrAssign, true, true)
            || opt_skip(TokenType::BitAndAssign, true, true)
            || opt_skip(TokenType::BitOrAssign, true, true)
            || opt_skip(TokenType::XorAssign, true, true)
            || opt_skip(TokenType::NullishAssign, true, true);
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
    void Parser::skip_nl(bool optional) {
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

    void Parser::skip(const TokenType & type, bool skip_l_nl, bool skip_r_nl, const std::string & expected, bool _virtual_semi) {
        if (skip_l_nl) {
            skip_nl(true);
        }
        if (is(type)) {
            advance();
        } else {
            expected_error(expected);
        }
        if (skip_r_nl) {
            skip_nl(true);
            virtual_semi = _virtual_semi ? true : virtual_semi;
        }
    }

    bool Parser::opt_skip(const TokenType & type, bool skip_l_nl, bool skip_r_nl) {
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
        log_parsing_entity("block");

        Position block_pos = peek().pos;
        tree::StmtList stmts;

        // One-line //
        // If one-line block is allowed then try to parse single stmt
        if (!is(TokenType::LBrace) && allow_one_line) {
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

        skip(TokenType::RBrace, true, true, "closing curly bracket '}' at block end", true);

        return std::make_shared<tree::Block>(block_pos, stmts);
    }

    // VarDecl //
    tree::stmt_ptr Parser::parse_var_decl() {
        log_parsing_entity("var_decl");

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
        log_parsing_entity("func_decl");

        Position func_decl_pos = peek().pos;

        skip(TokenType::Func, false, true, "'func' keyword");

        tree::id_ptr id = parse_id();

        tree::FuncParams params;
        bool using_parens = opt_skip(TokenType::LParen, true, true);

        bool first = true;
        while (!eof()) {
            skip_nl(true);
            // For using_paren break on paren
            // For no-paren break on `=>` (block start) or `->` (type anno) or `{` (block start)
            // Note: This syntax is inference-capable because params parsing stops on `=>` or `{` but return type may be expected...
            if (using_parens && is(TokenType::RParen)
            || is(TokenType::DoubleArrow) || is(TokenType::Arrow) || is(TokenType::LBrace)) {
                break;
            }
            if (first) {
                first = false;
            } else {
                skip(TokenType::Comma, true, true, "comma ',' to separate function parameters");
            }

            bool vararg = false;
            if (opt_skip(TokenType::Spread, true, true)) {
                vararg = true;
            }

            tree::id_ptr param_id = parse_id();

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

        // Find duplicates
        const auto & check_unique = std::unique(params.begin(), params.end());
        if (check_unique != params.end()) {
            error("Duplicate parameter name " + check_unique->id->get_name(), check_unique->id->pos);
        }

        if (using_parens) {
            skip(TokenType::RParen, true, true, "closing parenthesis ')' after parameter list");

            // Note: Not inference-capable syntax
            // Skip `->` or `:`
            if (!opt_skip(TokenType::Arrow, true, true)
            &&  !opt_skip(TokenType::Colon, true, true)) {
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
        log_parsing_entity("while_stmt");

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
        log_parsing_entity("for_stmt");

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
        log_parsing_entity("class_decl");

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

        skip(TokenType::RBrace, true, true, "closing curly bracket '}' at end of class body", true);

        return std::make_shared<tree::ClassDecl>(class_decl_pos, id, super, decls);
    }

    // Import //
    tree::stmt_ptr Parser::parse_import() {
        log_parsing_entity("import");

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
        log_parsing_entity("type_decl");

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
        tree::expr_ptr left = pipe();

        const auto & assign_op = peek();
        if (is_assign_op()) {
            log_parsing_entity("assignment");

            tree::expr_ptr value = parse_expr();

            if (left->type == tree::ExprType::Id) {
                tree::id_ptr id = std::static_pointer_cast<tree::Identifier>(left);
                return std::make_shared<tree::Assign>(id, value, assign_op);
            }

            if (left->type == tree::ExprType::Get) {
                std::shared_ptr<tree::GetExpr> get_expr = std::static_pointer_cast<tree::GetExpr>(left);
                return std::make_shared<tree::SetExpr>(get_expr->left, get_expr->id, assign_op, value);
            }

            if (left->type == tree::ExprType::GetItem) {
                std::shared_ptr<tree::GetItem> get_item = std::static_pointer_cast<tree::GetItem>(left);
                return std::make_shared<tree::SetItem>(get_item->left, get_item->index, assign_op, value);
            }

            unexpected_error();
        }

        return left;
    }

    tree::expr_ptr Parser::pipe() {
        tree::expr_ptr left = Or();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Pipe, true, true)) {
            log_parsing_entity("pipe");

            tree::expr_ptr right = Or();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::Or() {
        tree::expr_ptr left = And();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Or, true, true)) {
            log_parsing_entity("or");

            tree::expr_ptr right = And();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::And() {
        tree::expr_ptr left = bit_or();

        const auto & op_token = peek();
        while (opt_skip(TokenType::And, true, true)) {
            log_parsing_entity("and");

            tree::expr_ptr right = bit_or();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::bit_or() {
        tree::expr_ptr left = Xor();

        const auto & op_token = peek();
        while (opt_skip(TokenType::BitOr, true, true)) {
            log_parsing_entity("bit_or");

            tree::expr_ptr right = Xor();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::Xor() {
        tree::expr_ptr left = bit_and();

        const auto & op_token = peek();
        while (opt_skip(TokenType::BitAnd, true, true)) {
            log_parsing_entity("xor");

            tree::expr_ptr right = bit_and();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::bit_and() {
        tree::expr_ptr left = eq();

        const auto & op_token = peek();
        while (opt_skip(TokenType::BitAnd, true, true)) {
            log_parsing_entity("bit_and");

            tree::expr_ptr right = eq();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::eq() {
        tree::expr_ptr left = comp();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Eq, true, true)
            || opt_skip(TokenType::NotEq, true, true)
            || opt_skip(TokenType::RefEq, true, true)
            || opt_skip(TokenType::RefNotEq, true, true)) {
            log_parsing_entity("eq");

            tree::expr_ptr right = comp();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::comp() {
        tree::expr_ptr left = spaceship();

        const auto & op_token = peek();
        while (opt_skip(TokenType::LT, true, true)
            || opt_skip(TokenType::GT, true, true)
            || opt_skip(TokenType::LE, true, true)
            || opt_skip(TokenType::GE, true, true)) {
            log_parsing_entity("comp");

            tree::expr_ptr right = spaceship();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::spaceship() {
        tree::expr_ptr left = named_checks();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Cmp, true, true)) {
            log_parsing_entity("spaceship");

            tree::expr_ptr right = named_checks();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::named_checks() {
        tree::expr_ptr left = null_coalesce();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Is, true, true)
            || opt_skip(TokenType::NotIs, true, true)
            || opt_skip(TokenType::In, true, true)
            || opt_skip(TokenType::NotIn, true, true)) {
            log_parsing_entity("named_checks");

            tree::expr_ptr right = null_coalesce();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::null_coalesce() {
        tree::expr_ptr left = shift();

        const auto & op_token = peek();
        while (opt_skip(TokenType::NullCoalesce, true, true)) {
            log_parsing_entity("null_coalesce");

            tree::expr_ptr right = shift();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::shift() {
        tree::expr_ptr left = range();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Shr, true, true)
            || opt_skip(TokenType::Shl, true, true)) {
            log_parsing_entity("shift");

            tree::expr_ptr right = range();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::range() {
        tree::expr_ptr left = add();

        // TODO: Think if range to range is possible, now parse only `a..b` not `a..b..c`

        const auto & op_token = peek();
        if (opt_skip(TokenType::Range, true, true)
         || opt_skip(TokenType::RangeLE, true, true)
         || opt_skip(TokenType::RangeRE, true, true)
         || opt_skip(TokenType::RangeBothE, true, true)) {
            log_parsing_entity("range");

            tree::expr_ptr right = add();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::add() {
        tree::expr_ptr left = mult();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Add, true, true)
            || opt_skip(TokenType::Sub, true, true)) {
            log_parsing_entity("add");

            tree::expr_ptr right = mult();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::mult() {
        tree::expr_ptr left = power();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Mul, true, true)
            || opt_skip(TokenType::Div, true, true)
            || opt_skip(TokenType::Mod, true, true)) {
            log_parsing_entity("mult");

            tree::expr_ptr right = power();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::power() {
        tree::expr_ptr left = type_cast();

        const auto & op_token = peek();
        while (opt_skip(TokenType::Exp, true, true)) {
            log_parsing_entity("power");

            tree::expr_ptr right = type_cast();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::type_cast() {
        tree::expr_ptr left = prefix();

        const auto & op_token = peek();
        if (opt_skip(TokenType::As, true, true)
         || opt_skip(TokenType::AsQM, true, true)) {
            log_parsing_entity("type_cast");

            tree::expr_ptr right = prefix();
            left = std::make_shared<tree::Infix>(left, op_token, right);
        }

        return left;
    }

    tree::expr_ptr Parser::prefix() {
        const auto & op_token = peek();
        if (opt_skip(TokenType::Not, true, true)
         || opt_skip(TokenType::Sub, true, true)) {
            log_parsing_entity("prefix");

            tree::expr_ptr right = call();
            return std::make_shared<tree::Prefix>(op_token, right);
        }

        return call();
    }

    tree::expr_ptr Parser::call() {
        tree::expr_ptr left = member_access();

        while (!eof()) {
            // Note: For MethodCall too:
            // Fixme: Call after nl ?!
            //  Example:
            //  `function
            //  (...function_arguments)`
            if (is_after_nl(TokenType::LParen)) {
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
            if (opt_skip(TokenType::Dot, true, true)) {
                log_parsing_entity("get_expr");

                tree::id_ptr id = parse_id();

                if (is_after_nl(TokenType::LParen)) {
                    // We put left as nullptr, cause we already know left expression
                    const auto & func_call = std::static_pointer_cast<tree::FuncCall>(parse_func_call(nullptr));
                    left = std::make_shared<tree::MethodCall>(left, id, func_call->args);
                } else {
                    left = std::make_shared<tree::GetExpr>(left, id);
                }
            } else if (opt_skip(TokenType::LBracket, true, true)) {
                log_parsing_entity("sub-expression");

                tree::expr_ptr ind = parse_expr();
                skip(TokenType::RBracket, true, true, "closing bracket ']' after index access", true);
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

        // IfExpr //
        if (is(TokenType::If)) {
            return parse_if_expr();
        }

        Position primary_pos = peek().pos;

        // Grouping //
        if (opt_skip(TokenType::LParen, false, true)) {
            log_parsing_entity("grouping");

            tree::expr_ptr expr = parse_expr();

            skip(TokenType::RParen, true, true, "closing parenthesis ')' at end of grouping", true);

            return std::make_shared<tree::Grouping>(primary_pos, expr);
        }

        // ListExpr //
        if (opt_skip(TokenType::LBracket, false, true)) {
            log_parsing_entity("list");

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
                const auto & pos = peek().pos;
                // ...element
                if (opt_skip(TokenType::Spread, true, true)){
                    const auto & expr = parse_expr();
                    elements.push_back(std::make_shared<tree::SpreadExpr>(pos, expr));
                } else {
                    elements.push_back(parse_expr());
                }
            }
            skip(TokenType::RBracket, true, true, "closing bracket ']' after list literal", true);
            return std::make_shared<tree::ListExpr>(primary_pos, elements);
        }

        // Dictionary
        if (opt_skip(TokenType::LBrace, false, true)) {
            log_parsing_entity("dict");

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
            skip(TokenType::RBrace, true, true, "closing curly bracket '}' after dictionary literal", true);
            return std::make_shared<tree::DictExpr>(primary_pos, elements);
        }

        expected_error("primary expression");

        return nullptr;
    }

    // Identifier //
    tree::id_ptr Parser::parse_id() {
        log_parsing_entity("id");

        if (!is_after_nl(TokenType::Id)) {
            expected_error("identifier");
        }

        tree::id_ptr id = std::make_shared<tree::Identifier>(peek());
        skip(TokenType::Id, true, true, "", true);
        return id;
    }

    // FuncCall //
    tree::expr_ptr Parser::parse_func_call(const tree::expr_ptr & left) {
        log_parsing_entity("func_call");

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
            // Note: FuncCall does not allow trailing comma

            bool spread = false;
            if (opt_skip(TokenType::Spread, false, true)) {
                spread = true;
            }
            args.push_back({parse_expr(), spread});
        }

        skip(TokenType::RParen, true, true, "closing parenthesis ')'", true);

        return std::make_shared<tree::FuncCall>(left, args);
    }

    // IfExpr //
    tree::expr_ptr Parser::parse_if_expr() {
        log_parsing_entity("if_expr");

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
        log_parsing_entity("literal");

        Token current = peek();
        skip(current.type, false, true, "", true);
        return std::make_shared<tree::Literal>(current);
    }

    ///////////
    // Types //
    ///////////
    tree::type_ptr Parser::parse_type(const std::string & expected_type) {
        const auto & pos = peek().pos;
        tree::type_ptr left;
        if (is(TokenType::Id)) {
            log_parsing_entity("id_type");

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
                    types.push_back(parse_type("generic type argument"));
                    skip_nl(true);
                }
                skip(TokenType::GT, true, true, "'>' to end generic type arguments", true);
                left = std::make_shared<tree::GenericType>(id_type, types);
            } else {
                left = id_type;
            }
        } else if (opt_skip(TokenType::LBracket, false, true)) {
            log_parsing_entity("list_type");

            left = std::make_shared<tree::ListType>(pos, parse_type("list type"));
            skip(TokenType::RBracket, true, true, "closing bracket ']' at end of list type", true);
        } else if (opt_skip(TokenType::LBrace, true, true)) {
            log_parsing_entity("dict_type");

            const auto & key = parse_type("dictionary key type");
            skip(TokenType::Colon, true, true, "colon ':' to separate key and value in dictionary type");
            const auto & val = parse_type("dictionary value type");
            left = std::make_shared<tree::DictType>(pos, key, val);

            skip(TokenType::RBrace, true, true, "closing curly bracket '}' at end of dictionary type", true);
        } else {
            expected_error(expected_type);
        }

        if (opt_skip(TokenType::BitOr, true, true)) {
            log_parsing_entity("union_type");

            return std::make_shared<tree::UnionType>(left, parse_type("union type right-hand side"));
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
        if (eof()) {
            throw ExpectedException(expected, "[EOF]", tokens.at(index - 1).pos);
        }
        throw ExpectedException(expected, peek());
    }

    ///////////
    // Debug //
    ///////////
    void Parser::log_parsing_entity(const std::string & entity) {
        if (!options.log_parsing_entity) {
            return;
        }
        log.debug("Parse", "'" + entity + "'");
    }
}
