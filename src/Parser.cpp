#include "Parser.h"

Token Parser::peek(){
    return tokens[index];
}

Token Parser::advance(){
    return tokens[++index];
}

/////////////
// Chekers //
/////////////
bool Parser::eof(){
    return is_typeof(TokenType::Eof);
}

bool Parser::is_typeof(const TokenType & type){
    return peek().type == type;
}

bool Parser::is_nl(){
    return is_typeof(TokenType::Nl);
}

bool Parser::is_semis(){
    return is_nl() || is_op(Operator::Semi);
}

bool Parser::is_op(const Operator & op){
    return is_typeof(TokenType::Op) && peek().op() == op;
}

bool Parser::is_kw(const Keyword & kw){
    return is_typeof(TokenType::Kw) && peek().kw() == kw;
}

//////////////
// Skippers //
//////////////
void Parser::skip_nl(const bool & optional){
    if(is_nl()){
        do{
            advance();
        }while(is_nl());
    }else if(!optional){
        expected_error("[new line]");
    }
}

void Parser::skip_semis(){
    if(is_semis()){
        do{
            advance();
        }while(is_semis());
    }else{
        expected_error("`;` or [new line]");
    }
}

void Parser::skip_op(const Operator & op, const bool & skip_l_nl, const bool & skip_r_nl){
    if(skip_l_nl){
        skip_nl(true);
    }
    if(is_op(op)){
        advance();
    }else{
        expected_error("`"+ op_to_str(op) + "`");
    }
    if(skip_r_nl){
        skip_nl(true);
    }
}

void Parser::skip_kw(const Keyword & kw, const bool & skip_l_nl, const bool & skip_r_nl){
    if(skip_l_nl){
        skip_nl(true);
    }
    if(is_kw(kw)){
        advance();
    }else{
        expected_error("`"+ kw_to_str(kw) +"`");
    }
    if(skip_r_nl){
        skip_nl(true);
    }
}

/////////////
// Parsers //
/////////////
StmtList Parser::parse(const TokenStream & tokens){
    tree.clear();
    index = 0;
    this->tokens = tokens;

    while(!eof()){
        while(is_nl()){
            advance();
        }
        tree.push_back(parse_stmt());

        if(!eof()){
            skip_semis();
        }
    }

    return tree;
}

////////////////
// Statements //
////////////////
stmt_ptr Parser::parse_stmt(){

    if(is_typeof(TokenType::Kw)){
        switch(peek().kw()){
            case Keyword::Var:
            case Keyword::Val:{
                return parse_var_decl();
                break;
            }
            case Keyword::Func:{
                return parse_func_decl();
                break;
            }
            case Keyword::While:{
                return parse_while_stmt();
                break;
            }
            case Keyword::Return:{
                Position return_stmt_pos = peek().pos;
                advance();
                expr_ptr expr = nullptr;
                if(!is_semis()){
                    // not empty return
                    expr = parse_expr();
                }
                return std::make_shared<ReturnStmt>(return_stmt_pos, expr);
            }
            case Keyword::Class:{
                return parse_class_decl();
            }
        }
    }

    return std::make_shared<ExprStmt>(peek().pos, parse_expr());
}

// Block //
block_ptr Parser::parse_block(bool allow_one_line){
    Position block_pos = peek().pos;
    StmtList stmts;

    // One-line //
    // If one-line block is allowed then try to parse single stmt
    if(!is_op(Operator::LBrace) && allow_one_line){
        // TODO: Think about this skip_nl
        // Is it okay?
        skip_nl(true);
        stmts.push_back(parse_stmt());

        // Note: there's no need to expect semis after one-line block
        // because block is always part of statement and after all statements
        // there must be semi

        return std::make_shared<Block>(block_pos, stmts);
    }    

    // Multi-line //
    skip_op(Operator::LBrace, false, true);
    
    bool first = true;
    while(!eof()){
        if(is_op(Operator::RBrace)){
            break;
        }
        if(first){
            first = false;
        }else{
            skip_semis();
        }
        if(is_op(Operator::RBrace)){
            break;
        }
        stmts.push_back(parse_stmt());
    }
    skip_op(Operator::RBrace, true, false);

    return std::make_shared<Block>(block_pos, stmts);
}

// VarDecl //
stmt_ptr Parser::parse_var_decl(){
    Position var_decl_pos = peek().pos;

    VarDeclType decl = VarDeclType::Var;
    if(is_kw(Keyword::Var)){
        decl = VarDeclType::Var;
    }else if(is_kw(Keyword::Val)){
        decl = VarDeclType::Val;
    }else{
        expected_error("`var` or `val` keyword");
    }
    advance();

    id_ptr id = parse_id();

    expr_ptr assign_expr = nullptr;
    if(is_op(Operator::Assign)){
        skip_op(Operator::Assign, true, true);
        assign_expr = parse_expr();
    }

    return std::make_shared<VarDecl>(var_decl_pos, decl, id, assign_expr);
}

// FuncDecl //
stmt_ptr Parser::parse_func_decl(){
    Position func_decl_pos = peek().pos;

    skip_kw(Keyword::Func, false, true);
    id_ptr id = parse_id();

    bool paren = true;
    if(is_op(Operator::LParen)){
        skip_op(Operator::LParen, true, true);
    }else{
        paren = false;
    }
    
    FuncParams params;
    bool first = true;
    while(!eof()){
        if((paren && is_op(Operator::RParen))
        || (!paren && (is_op(Operator::Arrow) || is_op(Operator::LBrace)))){
            break;
        }
        if(first){
            first = false;
        }else{
            skip_op(Operator::Comma, true, true);
        }
        id_ptr param_id = parse_id();

        // Check for default value
        expr_ptr default_val = nullptr;
        if(is_op(Operator::Assign)){
            skip_op(Operator::Assign, true, true);
            default_val = parse_expr();
        }

        params.push_back({ param_id, default_val });
    }

    bool allow_one_line = false;
    if(paren){
        skip_op(Operator::RParen, true, true);
    }

    if(is_op(Operator::Arrow)){
        skip_op(Operator::Arrow, true, true);
        allow_one_line = true;
    }

    block_ptr body = parse_block(allow_one_line);

    return std::make_shared<FuncDecl>(func_decl_pos, id, params, body);
}

// WhileStmt //
stmt_ptr Parser::parse_while_stmt(){
    Position while_pos = peek().pos;

    skip_kw(Keyword::While, false, true);

    bool paren = true;
    if(is_op(Operator::LParen)){
        skip_op(Operator::LParen, true, true);
    }else{
        paren = false;
    }

    expr_ptr cond = parse_expr();

    bool allow_one_line = false;
    if(paren){
        skip_op(Operator::RParen, true, true);
        allow_one_line = true;
    }else if(is_nl()){
        allow_one_line = true;
    }

    if(is_op(Operator::Arrow)){
        skip_op(Operator::Arrow, true, true);
        allow_one_line = true;
    }

    block_ptr body = parse_block(allow_one_line);

    return std::make_shared<WhileStmt>(while_pos, cond, body);
}

// ClassDecl //
stmt_ptr Parser::parse_class_decl(){
    Position class_decl_pos = peek().pos;

    skip_kw(Keyword::Class, false, true);

    id_ptr id = parse_id();

    skip_nl(true);

    id_ptr super_id = nullptr;
    if(is_op(Operator::Colon)){
        skip_op(Operator::Colon, true, true);
        super_id = parse_id();
    }

    skip_op(Operator::LBrace, true, true);

    // Parse declarations
    // Note: Think about nested classes
    
    StmtList decls;
    while(!eof()){
        skip_nl(true);

        if(is_op(Operator::RBrace)){
            break;
        }

        if(is_kw(Keyword::Val) || is_kw(Keyword::Var)){
            decls.push_back(parse_var_decl());
        }else if(is_kw(Keyword::Func)){
            decls.push_back(parse_func_decl());
        }else{
            expected_error("function or variable declaration");
        }
        skip_semis();
    }

    skip_op(Operator::RBrace, true, false);

    return std::make_shared<ClassDecl>(class_decl_pos, id, super_id, decls);
}


/////////////////
// Expressions //
/////////////////
expr_ptr Parser::parse_expr(){
    return assignment();
}

expr_ptr Parser::assignment(){
    Position pos = peek().pos;

    expr_ptr expr = Or();

    // TODO: Add compound assignment operators

    if(is_op(Operator::Assign)){
        advance();

        expr_ptr value = parse_expr();

        if(expr->type == ExprType::Id){
            id_ptr id = std::static_pointer_cast<Identifier>(expr);
            return std::make_shared<Assign>(pos, id, value);
        }

        if(expr->type == ExprType::Get){
            std::shared_ptr<GetExpr> get_expr = std::static_pointer_cast<GetExpr>(expr);
            return std::make_shared<SetExpr>(pos, get_expr->left, get_expr->id, value);
        }

        unexpected_error();
    }

    return expr;
}

expr_ptr Parser::Or(){
    expr_ptr left = And();

    while(is_op(Operator::Or)){
        const auto op_token = peek();
        advance();
        expr_ptr right = And();
        left = std::make_shared<Infix>(op_token.pos, left, op_token, right);
    }

    return left;
}

expr_ptr Parser::And(){
    expr_ptr left = eq();

    while(is_op(Operator::And)){
        const auto op_token = peek();
        advance();
        expr_ptr right = eq();
        left = std::make_shared<Infix>(op_token.pos, left, op_token, right);
    }

    return left;
}

expr_ptr Parser::eq(){
    expr_ptr left = comp();

    while(is_op(Operator::Eq) || is_op(Operator::NotEq)){
        const auto op_token = peek();
        advance();
        expr_ptr right = comp();
        left = std::make_shared<Infix>(op_token.pos, left, op_token, right);
    }

    return left;
}

expr_ptr Parser::comp(){
    expr_ptr left = add();

    while(is_op(Operator::LT)
       || is_op(Operator::GT)
       || is_op(Operator::LE)
       || is_op(Operator::GE))
    {
        const auto op_token = peek();
        advance();
        expr_ptr right = add();
        left = std::make_shared<Infix>(op_token.pos, left, op_token, right);
    }

    return left;
}

expr_ptr Parser::add(){
    expr_ptr left = mult();

    while(is_op(Operator::Add) || is_op(Operator::Sub)){
        const auto op_token = peek();
        advance();
        expr_ptr right = mult();
        left = std::make_shared<Infix>(op_token.pos, left, op_token, right);
    }

    return left;
}

expr_ptr Parser::mult(){
    expr_ptr left = prefix();

    while(is_op(Operator::Mul) || is_op(Operator::Div)){
        const auto op_token = peek();
        advance();
        expr_ptr right = prefix();
        left = std::make_shared<Infix>(op_token.pos, left, op_token, right);
    }

    return left;
}

expr_ptr Parser::prefix(){
    if(is_op(Operator::Not) || is_op(Operator::Sub)){
        const auto op_token = peek();
        advance();
        expr_ptr right = prefix();
        return std::make_shared<Prefix>(op_token.pos, op_token, right);
    }
    
    return postfix();
}

expr_ptr Parser::postfix(){
    // Postfix is not only `Postfix` operator
    // It's also function call, member access, array access.
    Position pos = peek().pos;

    expr_ptr left = primary();

    while(true){
        if(is_op(Operator::LParen)){
            left = parse_func_call(left);
        }else if(is_op(Operator::Dot)){
            advance();
            id_ptr id = parse_id();
            left = std::make_shared<GetExpr>(pos, left, id);
        }else{
            break;
        }

        // TODO: Postfix operators
    }

    return left;
}

expr_ptr Parser::primary(){
    // Literal
    if(is_typeof(TokenType::Int)
    || is_typeof(TokenType::Float)
    || is_typeof(TokenType::Str)
    || is_typeof(TokenType::Bool)
    || is_typeof(TokenType::Null))
    {
        Token current = peek();
        advance();
        return std::make_shared<Literal>(current);
    }

    // Identifier
    if(is_typeof(TokenType::Id)){
        return parse_id();
    }

    // Grouping
    if(is_op(Operator::LParen)){
        skip_op(Operator::LParen, false, true);
        expr_ptr expr = parse_expr();
        skip_op(Operator::RParen, true, false);

        // TODO: !!! Think do I need special node for grouping? (precedence problem?) 
        return expr;
    }

    // If expression
    if(is_kw(Keyword::If)){
        return parse_if_expr();
    }

    unexpected_error();

    return nullptr;
}

// Identifier //
id_ptr Parser::parse_id(){
    if(!is_typeof(TokenType::Id)){
        expected_error("identifier");
    }

    id_ptr id = std::make_shared<Identifier>(peek());
    advance();
    return id;
}


// FuncCall //
expr_ptr Parser::parse_func_call(expr_ptr left){
    Position func_call_pos = peek().pos;

    skip_op(Operator::LParen, true, true);

    ExprList args;
    bool first = true;

    while(!eof()){
        if(is_op(Operator::RParen)){
            break;
        }
        if(first){
            first = false;
        }else{
            skip_op(Operator::Comma, true, true);
        }
        args.push_back(parse_expr());
    }

    skip_op(Operator::RParen, true, false);

    return std::make_shared<FuncCall>(func_call_pos, left, args);
}

// IfExpr //
expr_ptr Parser::parse_if_expr(){
    Position if_pos = peek().pos;

    skip_kw(Keyword::If, false, true);
    
    bool paren = true;
    if(is_op(Operator::LParen)){
        skip_op(Operator::LParen, true, true);
    }else{
        paren = false;
    }

    expr_ptr cond = parse_expr();

    bool allow_one_line = false;
    if(paren){
        skip_op(Operator::RParen, true, true);
        allow_one_line = true;
    }else if(is_nl()){
        // If `if` condition is not capture in parenthesis,
        // then only if there's new-line after it the body can be one-line
        allow_one_line = true;
    }

    if(is_op(Operator::Arrow)){
        skip_op(Operator::Arrow, true, true);
        allow_one_line = true;
    }

    block_ptr then_branch = parse_block(allow_one_line);

    skip_nl(true);

    block_ptr else_branch = nullptr;
    if(is_kw(Keyword::Else)){
        skip_kw(Keyword::Else, true, true);
        else_branch = parse_block(true);
    }

    return std::make_shared<IfExpr>(if_pos, cond, then_branch, else_branch);
}

////////////
// Errors //
////////////
void Parser::error(const std::string & msg){
    throw YoctoException(msg);
}

void Parser::unexpected_error(){
    throw UnexpectedException(peek());
}

void Parser::expected_error(const std::string & expected){
    throw ExpectedException(expected, peek());
}