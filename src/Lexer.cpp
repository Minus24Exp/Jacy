#include "Lexer.h"

Lexer::Lexer(const std::string & script){
	this->script = script;
	index = 0;
	line = 1;
	column = 1;
}

char Lexer::peek(){
	return script[index];
}

char Lexer::peekNext(){
	return script[index + 1];
}

char Lexer::advance(){
	index++;
	if(peek() == '\n'){
		line++;
		column = 1;
	}else{
		column++;
	}
	return peek();
}

bool Lexer::eof(){
	return peek() == '\0' || index >= script.size();
}

bool Lexer::skip(const char & c){
	return c == '\t' || c == ' ' || c == '\r';
}

bool Lexer::is_nl(const char & c){
	return c == '\n';
}

bool Lexer::is_digit(const char & c){
	return c >= '0' && c <= '9';
}

bool Lexer::is_hex(const char & c){
	return is_digit(c)
		|| (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F');
}

bool Lexer::is_id_first(const char & c){
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c == '_');
}

bool Lexer::is_id(const char & c){
	return is_id_first(c) || is_digit(c);
}

bool Lexer::is_quote(const char & c){
	return c == '"' || c == '\'' || c == '`';
}

void Lexer::add_token(Token t){
	t.pos.line = token_line;
	t.pos.column = token_column;

	tokens.push_back(t);
}

void Lexer::add_token(const TokenType & type, const std::string & val){
	add_token(Token(type, val));
}

void Lexer::add_token(const Operator & op){
	add_token(Token(op));
}

void Lexer::add_token(const Keyword & kw){
	add_token(Token(kw));
}

void Lexer::add_token(const TokenType & type){
	add_token(Token(type));
}

void Lexer::add_token(const int & i){
	add_token(Token(i));
}

void Lexer::add_token(const double & d){
	add_token(Token(d));
}

void Lexer::lex_number(){
	TokenType num_type = TokenType::Int;
	std::string num;

	if(peek() == '0'){
		advance();
		switch(peek()){
			case 'x':
			case 'X':{
				advance();
				if(!is_hex(peek())){
					unexpected_error();
				}
				do{
					num += peek();
				}while(is_hex(advance()));

				add_token(std::stoi(num, 0, 16));
				return;
				break;
			}
			case 'b':
			case 'B':{
				advance();
				if(!is_digit(peek())){
					unexpected_error();
				}
				do{
					num += peek();
				}while(is_digit(advance()));

				add_token(std::stoi(num, 0, 2));
				return;
				break;
			}
			default: num = "0";
		}
	}

	// TODO: Fix floating hex numbers

	while(is_digit(peek())){
		num += peek();
		advance();
	}

	if(peek() == '.'){
		num_type = TokenType::Float;
		num += peek();
		advance();
		if(!is_digit(peek())){
			unexpected_error();
		}
		do{
			num += peek();
		}while(is_digit(advance()));
	}

	if(num_type == TokenType::Float){
		add_token(std::stod(num));
	}else{
		add_token(std::stoi(num));
	}
}

TokenStream Lexer::lex(){

	while(!eof()){
		token_line = line;
		token_column = column;
	
		if(skip(peek())){
			advance();
		}else if(is_nl(peek())){
			add_token(TokenType::Nl);
			advance();
		}else if(is_digit(peek())){
			lex_number();
		}else if(is_id_first(peek())){
			std::string id(1, peek());
			while(is_id(advance())){
				id += peek();
			}

			Keyword kw = str_to_kw(id);

			if(kw < Keyword::MAX){
				add_token(kw);
			}else{
				add_token(TokenType::Id, id);
			}
		}else if(is_quote(peek())){
			const char quote = peek();
			advance();
			std::string str = "";
			while(!eof()){
				if(peek() == quote){
					break;
				}
				str += peek();
				advance();
			}
			if(eof()){
				unexpected_eof_error();
			}
			if(peek() != quote){
				unexpected_error();
			}
			add_token(TokenType::Str, str);
			advance();
		}else{
			switch(peek()){
				case '=':{
					add_token(Operator::Assign);
					advance();
					break;
				}
				case '+':{
					add_token(Operator::Add);
					advance();
					break;
				}
				case '-':{
					add_token(Operator::Sub);
					advance();
					break;
				}
				case '*':{
					add_token(Operator::Mul);
					advance();
					break;
				}
				case '/':{
					if(peekNext() == '/'){
						while(!eof()){
							advance();
							if(is_nl(peek())){
								break;
							}
						}
					}else if(peekNext() == '*'){
						while(!eof()){
							advance();
							if(peek() == '*' && peekNext() == '/'){
								break;
							}
						}
						advance(); // Skip `*`
						advance(); // Skip `/`
					}else{
						add_token(Operator::Div);
						advance();
					}
					break;
				}
				case '%':{
					add_token(Operator::Mod);
					advance();
					break;
				}
				case ';':{
					add_token(Operator::Semi);
					advance();
					break;
				}
				case '(':{
					add_token(Operator::LParen);
					advance();
					break;
				}
				case ')':{
					add_token(Operator::RParen);
					advance();
					break;
				}
				case '{':{
					add_token(Operator::LBrace);
					advance();
					break;
				}
				case '}':{
					add_token(Operator::RBrace);
					advance();
					break;
				}
				case ',':{
					add_token(Operator::Comma);
					advance();
					break;
				}
				case ':':{
					add_token(Operator::Colon);
					advance();
					break;
				}
				case '.':{
					if(is_digit(peekNext())){
						lex_number();
					}else{
						add_token(Operator::Dot);
						advance();
					}
					break;
				}
				default:{
					unexpected_error();
				}
			}
		}
	}

	add_token(TokenType::Eof);

	return tokens;
}

void Lexer::unexpected_error(){
	std::string error = "Unexpected token `"+ std::string(1, peek()) + "`";
	error += " at "+ std::to_string(line) +":"+ std::to_string(column);
	throw YoctoException(error);
}

void Lexer::unexpected_eof_error(){
	throw UnexpectedEofException();
}