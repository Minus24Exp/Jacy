#include "Printer.h"

Printer::Printer(){}

void Printer::print_indent(){
	// Note: Use print indent only before statement printing
	// not before expression
	std::string str(indent * 4, ' ');
	std::cout << str;
}

void Printer::print(const StmtList & tree){
	indent = 0;
	for(int i = 0; i < tree.size(); i++){
		tree[i]->accept(*this);
		if(i < tree.size() - 1){
			std::cout << std::endl;
		}
	}
}

void Printer::visit(ExprStmt * expr_stmt){
	print_indent();
	expr_stmt->expr->accept(*this);
}

void Printer::visit(Literal * literal){
	switch(literal->token.type){
		case TokenType::Bool: std::cout << literal->token.Bool(); break;
		case TokenType::Int: std::cout << literal->token.Int(); break;
		case TokenType::Float: std::cout << literal->token.Float(); break;
		case TokenType::Str: std::cout << "\"" << literal->token.String() << "\""; break;
	}
}

void Printer::visit(Identifier * id){
	std::cout << id->token.String();
}

void Printer::visit(VarDecl * var_decl){
	print_indent();
	if(var_decl->decl == VarDeclType::Var){
		std::cout << "var";
	}else if(var_decl->decl == VarDeclType::Val){
		std::cout << "val";
	}
	std::cout << " ";
	
	visit(var_decl->id.get());

	if(var_decl->assign_expr){
		std::cout << " = ";
		var_decl->assign_expr->accept(*this);
	}
}

void Printer::visit(Block * block){
	std::cout << "{\n";
	indent++;
	for(int i = 0; i < block->stmts.size(); i++){
		block->stmts[i]->accept(*this);
		if(i < block->stmts.size() - 1){
			std::cout << std::endl;
		}
	}
	indent--;
	std::cout << std::endl;
	print_indent();
	std::cout << "}";
}

void Printer::visit(FuncDecl * func_decl){
	print_indent();
	std::cout << "func ";
	func_decl->id->accept(*this);

	std::cout << "(";
	for(int i = 0; i < func_decl->params.size(); i++){
		func_decl->params[i].id->accept(*this);
		if(i < func_decl->params.size() - 1){
			std::cout << ", ";
		}
	}
	std::cout << ")";

	func_decl->body->accept(*this);
}

void Printer::visit(FuncCall * func_call){
	func_call->left->accept(*this);

	std::cout << "(";
	for(int i = 0; i < func_call->args.size(); i++){
		func_call->args[i]->accept(*this);
		if(i < func_call->args.size() - 1){
			std::cout << ", ";
		}
	}
	std::cout << ")";
}

void Printer::visit(InfixOp * infix_op){
	std::cout << "(";
	infix_op->left->accept(*this);
	std::cout << " " << op_to_str(infix_op->op.op()) << " ";
	infix_op->right->accept(*this);
	std::cout << ")";
}

void Printer::visit(IfExpression * if_expr){
	bool first = true;
	for(const auto & c : if_expr->conditions){
		if(first){
			std::cout << "if";
			first = false;
		}else{
			std::cout << "elif";
		}
		std::cout << " (";
		c.cond->accept(*this);
		std::cout << ") ";
		c.body->accept(*this);
	}

	if(if_expr->Else){
		std::cout << " else {\n";
		if_expr->Else->accept(*this);
		std::cout << "\n}";
	}
}