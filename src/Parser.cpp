#include "Parser.h"

Parser::Parser(const TokenStream & tokens){
	index = 0;
	this->tokens = tokens;
}

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

bool Parser::is_op(const Operator & op){
	return is_typeof(TokenType::Op) && peek().op() == op;
}

bool Parser::is_kw(const Keyword & kw){
	return is_typeof(TokenType::Kw) && peek().kw() == kw;
}

bool Parser::is_infix_op(){
	return is_typeof(TokenType::Op) && get_infix_prec(peek().op()) != static_cast<int>(InfixPrec::None);
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
	if(is_nl() || is_op(Operator::Semi)){
		do{
			advance();
		}while(is_nl() || is_op(Operator::Semi));
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
StmtList Parser::parse(){
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
				return parse_while();
				break;
			}
		}
	}

	return std::make_shared<ExprStmt>(peek().pos, parse_expr());
}

expr_ptr Parser::parse_expr(){
	expr_ptr left = parse_atom();

	while(!eof()){
		if(is_op(Operator::LParen)){
			left = parse_func_call(left);
		}else if(is_infix_op()){
			return parse_infix(left, 0);
			// TODO: Add else if postfix (and break!)
		}else{
			break;
		}
	}
	
	return left;
}

expr_ptr Parser::parse_atom(){
	if(is_typeof(TokenType::Int) || is_typeof(TokenType::Float) || is_typeof(TokenType::Str) || is_typeof(TokenType::Bool)){
		Token current = peek();
		advance();
		return std::make_shared<Literal>(current);
	}

	if(is_typeof(TokenType::Id)){
		return parse_id();
	}

	if(is_kw(Keyword::If)){
		return parse_if_expr();
	}

	unexpected_error();
	return nullptr;
}

id_ptr Parser::parse_id(){
	if(!is_typeof(TokenType::Id)){
		expected_error("identifier");
	}

	id_ptr id = std::make_shared<Identifier>(peek());
	advance();
	return id;
}

block_ptr Parser::parse_block(bool allow_one_line){
	Position block_start = peek().pos;
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

		return std::make_shared<Block>(block_start, stmts);
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

	return std::make_shared<Block>(block_start, stmts);
}

expr_ptr Parser::parse_infix(expr_ptr left, int prec){
	// Note: For infix, position is pretty useless, because all
	// errors will be about left, op or right, and they have
	// their own positions

	if(is_infix_op()){
		Token op_token = peek();
		int right_prec = get_infix_prec(op_token.op());
		if(right_prec > prec){
			advance();
			expr_ptr maybe_infix = std::make_shared<Infix>(op_token.pos, left, op_token, parse_infix(parse_atom(), right_prec));
			return parse_infix(maybe_infix, prec);
		}
	}

	return left;
}

stmt_ptr Parser::parse_var_decl(){
	Position var_decl_start = peek().pos;

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

	return std::make_shared<VarDecl>(var_decl_start, decl, id, assign_expr);
}

stmt_ptr Parser::parse_func_decl(){
	Position func_decl_start = peek().pos;

	skip_kw(Keyword::Func, false, true);
	id_ptr id = parse_id();

	bool paren = true;
	if(is_op(Operator::LParen)){
		skip_op(Operator::LParen, true, true);
	}else{
		paren = false;
	}
	
	ParamList params;
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

	return std::make_shared<FuncDecl>(func_decl_start, id, params, body);
}

expr_ptr Parser::parse_func_call(expr_ptr left){
	Position func_call_start = peek().pos;

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

	return std::make_shared<FuncCall>(func_call_start, left, args);
}

expr_ptr Parser::parse_if_expr(){
	Position if_start = peek().pos;

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

	return std::make_shared<IfExpr>(if_start, cond, then_branch, else_branch);
}

stmt_ptr Parser::parse_while(){
	Position while_start = peek().pos;

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

	return std::make_shared<While>(while_start, cond, body);
}

////////////
// Errors //
////////////
void Parser::error(const std::string & msg){
	throw YoctoException(msg);
}

void Parser::unexpected_error(){
	throw UnexpectedTokenException(peek());
}

void Parser::expected_error(const std::string & expected){
	throw ExpectedException(expected, peek());
}