#include "parser/Parser.h"

Parser::Parser() : log("Parser", options.log) {}

Token Parser::peek() {
    return tokens[index];
}

Token Parser::advance() {
    return tokens[++index];
}

/////////////
// Chekers //
/////////////
bool Parser::eof() {
    return is(TokenType::Eof);
}

bool Parser::is(const TokenType & type) {
    return peek().type == type;
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
        || is(TokenType::ExpAssign);
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
        expected_error("`;` or [new line]");
    }
}

void Parser::skip(const TokenType & type, const bool & skip_l_nl, const bool & skip_r_nl) {
    if (skip_l_nl) {
        skip_nl(true);
    }
    if (is(type)) {
        advance();
    } else {
        // TODO: Add TokenType to string conversion by range
//        expected_error("`"+ op_to_str(op) + "`");
    }
    if (skip_r_nl) {
        skip_nl(true);
    }
}

/////////////
// Parsers //
/////////////
StmtList Parser::parse(const TokenStream & ts) {
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
stmt_ptr Parser::parse_stmt() {
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
            expr_ptr expr = nullptr;
            if (!is_semis()) {
                // not empty return
                expr = parse_expr();
            }
            return std::make_shared<ReturnStmt>(return_stmt_pos, expr);
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
            return std::make_shared<ExprStmt>(parse_expr());
        }
    }
}

// Block //
block_ptr Parser::parse_block(bool allow_one_line) {
    print_parsing_entity("block");

    Position block_pos = peek().pos;
    StmtList stmts;

    // One-line //
    // If one-line block is allowed then try to parse single stmt
    if (!is(TokenType::LBrace) && allow_one_line) {
        // @TODO: Think about this skip_nl
        // Is it okay?
        skip_nl(true);
        stmts.push_back(parse_stmt());

        // Note: there's no need to expect semis after one-line block
        // because block is always part of statement and after all statements
        // there must be semi

        return std::make_shared<Block>(block_pos, stmts);
    }

    // Multi-line //
    skip(TokenType::LBrace, false, true);

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

    skip(TokenType::RBrace, true, false);

    virtual_semi = true;

    return std::make_shared<Block>(block_pos, stmts);
}

// VarDecl //
stmt_ptr Parser::parse_var_decl() {
    print_parsing_entity("var_decl");

    Position var_decl_pos = peek().pos;

    VarDeclKind decl = VarDeclKind::Var;
    if (is(TokenType::Var)) {
        decl = VarDeclKind::Var;
    } else if (is(TokenType::Val)) {
        decl = VarDeclKind::Val;
    } else {
        expected_error("`var` or `val` keyword");
    }
    advance();

    id_ptr id = parse_id();

    expr_ptr assign_expr = nullptr;

    // It's obvious, but mark that augmented assignment cannot appear in variable declaration
    if (is(TokenType::Assign)) {
        skip(TokenType::Assign, true, true);
        assign_expr = parse_expr();
    }

    return std::make_shared<VarDecl>(var_decl_pos, decl, id, assign_expr);
}

// FuncDecl //
stmt_ptr Parser::parse_func_decl() {
    print_parsing_entity("func_decl");

    Position func_decl_pos = peek().pos;

    skip(TokenType::Func, false, true);

    id_ptr id = parse_id();

    FuncParams params;
    if (is(TokenType::LParen)) {
        skip(TokenType::LParen, true, true);

        bool first = true;
        while (!eof()) {
            if (is(TokenType::RParen)) {
                break;
            }
            if (first) {
                first = false;
            } else {
                skip(TokenType::Comma, true, true);
            }
            id_ptr param_id = parse_id();

            // Find duplicates
            // TODO: Think about checking after all parameters added
            for (const auto & param : params) {
                if (param.id->get_name() == param_id->get_name()) {
                    error("Parameter duplication");
                    return nullptr;
                }
            }

            // Check for default value
            expr_ptr default_val = nullptr;
            if (is(TokenType::Assign)) {
                skip(TokenType::Assign, true, true);
                default_val = parse_expr();
            }

            params.push_back({ param_id, default_val });
        }
        
        skip(TokenType::RParen, true, true);
    }

    bool allow_one_line = false;
    if (is(TokenType::Arrow)) {
        skip(TokenType::Arrow, true, true);
        allow_one_line = true;
    }

    block_ptr body = parse_block(allow_one_line);

    return std::make_shared<FuncDecl>(func_decl_pos, id, params, body);
}

// WhileStmt //
stmt_ptr Parser::parse_while_stmt() {
    print_parsing_entity("while_stmt");

    Position while_pos = peek().pos;

    skip(TokenType::While, false, false);

    expr_ptr cond = parse_expr();

    bool allow_one_line = false;
    if (is_nl()) {
        allow_one_line = true;
    }

    if (is(TokenType::Arrow)) {
        skip(TokenType::Arrow, true, true);
        allow_one_line = true;
    }

    block_ptr body = parse_block(allow_one_line);

    return std::make_shared<WhileStmt>(while_pos, cond, body);
}

// ForStmt //
stmt_ptr Parser::parse_for_stmt() {
    print_parsing_entity("for_stmt");

    Position for_stmt_pos = peek().pos;

    skip(TokenType::For, false, false);

    id_ptr For = parse_id();

    skip(TokenType::In, false, false);

    expr_ptr In = parse_expr();

    bool allow_one_line = false;
    if (is_nl()) {
        allow_one_line = true;
    }

    if (is(TokenType::Arrow)) {
        skip(TokenType::Arrow, true, true);
        allow_one_line = true;
    }

    block_ptr body = parse_block(allow_one_line);

    return std::make_shared<ForStmt>(for_stmt_pos, For, In, body);
}

// ClassDecl //
stmt_ptr Parser::parse_class_decl() {
    print_parsing_entity("class_decl");

    Position class_decl_pos = peek().pos;

    skip(TokenType::Class, false, true);

    id_ptr id = parse_id();

    skip_nl(true);

    expr_ptr super = nullptr;
    if (is(TokenType::Colon)) {
        skip(TokenType::Colon, true, true);
        super = parse_expr();
    }

    skip(TokenType::LBrace, true, true);

    // @Note: Think about nested classes
    
    // Parse declarations
    StmtList decls;
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

    skip(TokenType::RBrace, true, false);

    return std::make_shared<ClassDecl>(class_decl_pos, id, super, decls);
}

// Import //
stmt_ptr Parser::parse_import() {
    print_parsing_entity("import");

    Position import_pos = peek().pos;
    // No new-lines in import
    skip(TokenType::Import, false, false);

    // @TODO: Improve `import`
    // - Multiple objects import

    // Import nothing, just run source
    // e.g. `import "path"`
    if (is(TokenType::String)) {
        std::string path = peek().val;
        advance();
        return std::make_shared<Import>(import_pos, path);
    }

    // Complex import
    ImportEntityList entities;
    bool first = true;
    while (!eof()) {
        skip_nl(true);
        if (is_nl() || is(TokenType::From)) {
            break;
        }
        if (first) {
            first = false;
        } else {
            skip(TokenType::Comma, false, false);
        }
        if (is_nl() || is(TokenType::From)) {
            break;
        }

        bool all = false;
        id_ptr object = nullptr;
        id_ptr as = nullptr;
        if (is(TokenType::Mul)) {
            advance();
            all = true;
        } else {
            object = parse_id();
        }

        // All (`*`) requires `as` annotation
        if (all || is(TokenType::As)) {
            skip(TokenType::As, false, false);
            as = parse_id();
        } else {
            as = nullptr;
        }

        entities.push_back({ all, object, as });
    }

    skip(TokenType::From, false, false);

    if (!is(TokenType::String)) {
        expected_error("path to file (String)");
    }

    std::string path = peek().val;
    advance();

    return std::make_shared<Import>(import_pos, path, entities);
}

// TypeDecl //
stmt_ptr Parser::parse_type_decl() {
    print_parsing_entity("type_decl");

    Position type_decl_pos = peek().pos;

    skip(TokenType::Type, false, false);
    id_ptr id = parse_id();
    skip(TokenType::Assign, false, false);
    expr_ptr type_expr = parse_expr();

    return std::make_shared<TypeDecl>(type_decl_pos, id, type_expr);
}

/////////////////
// Expressions //
/////////////////
expr_ptr Parser::parse_expr() {
    return assignment();
}

expr_ptr Parser::assignment() {
    expr_ptr expr = pipe();

    // @TODO: Add compound assignment operators

    if (is_assign_op()) {
        print_parsing_entity("assignment");

        Token assign_op = peek();
        advance();

        // Fixme: skip_nl?

        expr_ptr value = parse_expr();

        if (expr->type == ExprType::Id) {
            id_ptr id = std::static_pointer_cast<Identifier>(expr);
            return std::make_shared<Assign>(id, value, assign_op);
        }

        if (expr->type == ExprType::Get) {
            std::shared_ptr<GetExpr> get_expr = std::static_pointer_cast<GetExpr>(expr);
            return std::make_shared<SetExpr>(get_expr->left, get_expr->id, value);
        }

        if (expr->type == ExprType::GetItem) {
            std::shared_ptr<GetItem> get_item = std::static_pointer_cast<GetItem>(expr);
            return std::make_shared<SetItem>(get_item->left, get_item->index, value);
        }

        unexpected_error();
    }

    return expr;
}

expr_ptr Parser::pipe() {
    expr_ptr left = Or();

    while (is(TokenType::Pipe)) {
        print_parsing_entity("pipe");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = Or();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::Or() {
    expr_ptr left = And();

    while (is(TokenType::Or)) {
        print_parsing_entity("or");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = And();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::And() {
    expr_ptr left = eq();

    while (is(TokenType::And)) {
        print_parsing_entity("and");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = eq();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::eq() {
    expr_ptr left = comp();

    while (is(TokenType::Eq) || is(TokenType::NotEq)
       || is(TokenType::RefEq) || is(TokenType::RefNotEq))
    {
        print_parsing_entity("eq");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = comp();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::comp() {
    expr_ptr left = named_checks();

    while (is(TokenType::LT)
       || is(TokenType::GT)
       || is(TokenType::LE)
       || is(TokenType::GE))
    {
        print_parsing_entity("comp");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = named_checks();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::named_checks() {
    expr_ptr left = range();

    while (is(TokenType::Is)
       || is(TokenType::NotIs)
       || is(TokenType::In)
       || is(TokenType::NotIn))
    {
        print_parsing_entity("named_checks");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = range();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::range() {
    expr_ptr left = add();

    // @TODO: Think if range to range is possible, now parse only `a..b` not `a..b..c`
    if (is(TokenType::Range)
    || is(TokenType::RangeLE)
    || is(TokenType::RangeRE)
    || is(TokenType::RangeBothE))
    {
        print_parsing_entity("range");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = add();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::add() {
    expr_ptr left = mult();

    while (is(TokenType::Add) || is(TokenType::Sub)) {
        print_parsing_entity("add");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = mult();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::mult() {
    expr_ptr left = power();

    while (is(TokenType::Mul) || is(TokenType::Div) || is(TokenType::Mod)) {
        print_parsing_entity("mult");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = power();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::power() {
    expr_ptr left = type_cast();

    while (is(TokenType::Exp)) {
        print_parsing_entity("power");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = type_cast();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::type_cast() {
    expr_ptr left = prefix();

    if (is(TokenType::As) || is(TokenType::AsQM)) {
        print_parsing_entity("type_cast");

        const auto & op_token = peek();
        advance();
        skip_nl(true);
        expr_ptr right = prefix();
        left = std::make_shared<Infix>(left, op_token, right);
    }

    return left;
}

expr_ptr Parser::prefix() {
    if (is(TokenType::Not) || is(TokenType::Sub)) {
        print_parsing_entity("prefix");

        const auto & op_token = peek();
        advance();
        expr_ptr right = call();
        return std::make_shared<Prefix>(op_token, right);
    }

    return call();
}

expr_ptr Parser::call() {
    expr_ptr left = member_access();

    while (!eof()) {
        if (is(TokenType::LParen)) {
            left = parse_func_call(left);
        } else {
            break;
        }
    }

    return left;
}

expr_ptr Parser::member_access() {
    expr_ptr left = primary();

    while (!eof()) {
        if (is(TokenType::Dot)) {
            print_parsing_entity("get_expr");

            advance();
            id_ptr id = parse_id();
            left = std::make_shared<GetExpr>(left, id);
        } else if (is(TokenType::LBracket)) {
            print_parsing_entity("sub-expression");

            skip(TokenType::LBracket, false, true);
            expr_ptr ind = parse_expr();
            skip(TokenType::RBracket, true, false);
            left = std::make_shared<GetItem>(left, ind);
        } else {
            break;
        }
    }

    return left;
}

expr_ptr Parser::primary() {
    // Literal
    if (is_literal()) {
        return parse_literal();
    }

    // Identifier
    if (is(TokenType::Id)) {
        return parse_id();
    }

    // Grouping
    if (is(TokenType::LParen)) {
        skip(TokenType::LParen, false, true);
        expr_ptr expr = parse_expr();
        skip(TokenType::RParen, true, false);

        // @TODO: !!! Think do I need special node for grouping? (precedence problem?) 
        return expr;
    }

    Position pos = peek().pos;

    // If expression
    if (is(TokenType::If)) {
        return parse_if_expr();
    }

    // List
    if (is(TokenType::LBracket)) {
        skip(TokenType::LBracket, false, true);

        ExprList elements;
        bool first = true;
        while (!eof()) {
            skip_nl(true);
            if (is(TokenType::RBracket)) {
                break;
            }
            if (first) {
                first = false;
            } else {
                skip(TokenType::Comma, true, true);
            }
            // Note: Allow `[1,]` (comma without next element)
            if (is(TokenType::RBracket)) {
                break;
            }
            elements.push_back(parse_expr());
        }
        skip(TokenType::RBracket, true, false);
        return std::make_shared<ListExpr>(pos, elements);
    }

    // Dictionary
    if (is(TokenType::LBrace)) {
        skip(TokenType::LBrace, false, true);

        DictElementList elements;
        bool first = true;
        while (!eof()) {
            skip_nl(true);
            if (is(TokenType::RBrace)) {
                break;
            }
            if (first) {
                first = false;
            } else {
                skip(TokenType::Comma, true, true);
            }
            if (is(TokenType::RBrace)) {
                break;
            }

            id_ptr id_key = nullptr;
            expr_ptr expr_key = nullptr;

            if (is(TokenType::LBracket)) {
                skip(TokenType::LBracket, true, true);
                expr_key = parse_expr();
                skip(TokenType::RBracket, true, true);
            } else if (is_literal()) {
                expr_key = parse_literal();
            } else {
                id_key = parse_id();
            }

            skip(TokenType::Colon, true, true);
            expr_ptr val = parse_expr();

            elements.push_back({id_key, expr_key, val});
        }
        skip(TokenType::RBrace, true, false);
        return std::make_shared<DictExpr>(pos, elements);
    }

    expected_error("primary expression");

    return nullptr;
}

// Identifier //
id_ptr Parser::parse_id() {
    print_parsing_entity("id");

    if (!is(TokenType::Id)) {
        expected_error("identifier");
    }

    id_ptr id = std::make_shared<Identifier>(peek());
    advance();
    return id;
}


// FuncCall //
expr_ptr Parser::parse_func_call(const expr_ptr & left) {
    print_parsing_entity("func_call");

    skip(TokenType::LParen, false, true);

    ExprList args;

    bool first = true;
    while (!eof()) {
        skip_nl(true);
        if (is(TokenType::RParen)) {
            break;
        }
        if (first) {
            first = false;
        } else {
            skip(TokenType::Comma, true, true);
        }
        if (is(TokenType::RParen)) {
            break;
        }
        args.push_back(parse_expr());
    }

    skip(TokenType::RParen, true, false);

    return std::make_shared<FuncCall>(left, args);
}

// IfExpr //
expr_ptr Parser::parse_if_expr() {
    print_parsing_entity("if_expr");

    Position if_pos = peek().pos;

    skip(TokenType::If, false, true);

    expr_ptr cond = parse_expr();

    bool allow_one_line = false;
    if (is_nl()) {
        // If `if` condition is not captured in parenthesis,
        // then only if there's new-line after it, body can be one-line
        allow_one_line = true;
    }

    if (is(TokenType::Arrow)) {
        skip(TokenType::Arrow, true, true);
        allow_one_line = true;
    }

    block_ptr then_branch = parse_block(allow_one_line);

    // Allow to write one-line expressions
    // like: val a = if true => 'yeps' else 'nope'
    if (!is(TokenType::Else) && !virtual_semi) {
        skip_semis();
        virtual_semi = true;
    }

    block_ptr else_branch = nullptr;
    if (is(TokenType::Else)) {
        skip(TokenType::Else, false, true);
        else_branch = parse_block(true);
    }

    return std::make_shared<IfExpr>(if_pos, cond, then_branch, else_branch);
}

expr_ptr Parser::parse_literal() {
    print_parsing_entity("literal");

    Token current = peek();
    advance();
    return std::make_shared<Literal>(current);
}

////////////
// Errors //
////////////
void Parser::error(const std::string & msg) {
    auto message = msg;
    message += " at " + std::to_string(peek().pos.column) + ":" + std::to_string(peek().pos.line);
    throw ParserException(message);
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
    log.debug("Parse " + entity);
}

