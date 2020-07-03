#include "backend/Interpreter.h"

Interpreter::Interpreter(){
	value = nullptr;
	scope = new Scope;
}

void Interpreter::interpret(const StmtList & tree){
	for(const auto & stmt : tree){
		execute(stmt.get());
	}
}

void Interpreter::execute(Statement * stmt){
	stmt->accept(*this);
}

obj_ptr Interpreter::eval(Expression * expr){
	expr->accept(*this);
	return std::move(value);
}

void Interpreter::execute_block(Block * block, scope_ptr sub_scope){
	scope_ptr previous = scope;
	scope = sub_scope;
	for(const auto & stmt : block->stmts){
		execute(stmt.get());
	}
	scope = previous;
}

void Interpreter::call(Func * func, ObjList && args){
	scope_ptr sub_scope = std::make_shared<Scope>(func->closure);
	scope_ptr previous = scope;

	for(size_t i = 0; i < func->decl.params.size(); i++){
		sub_scope->define(func->decl.params[i].id->get_name(), std::move(args[i]));
	}

	try{
		execute_block(func->decl.body.get(), sub_scope);
	}catch(Object & ret_val){
		scope = previous;
		return;
	}
}

void Interpreter::visit(ExprStmt * expr_stmt){
	value = eval(expr_stmt->expr.get());
}

void Interpreter::visit(Literal * literal){
	switch(literal->token.type){
		case TokenType::Null:{
			value.reset(new Null);
			break;
		}
		case TokenType::Bool:{
			value.reset(new Bool(literal->token.Bool()));
			break;
		}
		case TokenType::Int:{
			value.reset(new Int(literal->token.Int()));
			break;
		}
		case TokenType::Float:{
			value.reset(new Float(literal->token.Float()));
			break;
		}
		case TokenType::Str:{
			value.reset(new String(literal->token.String()));
			break;
		}
	}
}

void Interpreter::visit(Identifier * id){
	Object * obj = scope->get(id->get_name());
	if(!obj){
		throw YoctoException(id->get_name() + " is not defined");
		return;
	}
	value = obj->clone();
}

void Interpreter::visit(VarDecl * var_decl){
	std::cout << "visit var_decl" << std::endl;
	if(var_decl->assign_expr){
		value = eval(var_decl->assign_expr.get());
		scope->define(var_decl->id->get_name(), value->clone());
	}else{
		scope->define(var_decl->id->get_name(), std::make_unique<Null>());
	}

	value = nullptr;
}

void Interpreter::visit(Block * block){
	scope_ptr sub_scope = std::make_shared<Scope>(scope);
	execute_block(block, sub_scope);
}

void Interpreter::visit(FuncDecl * func_decl){
	obj_ptr func = std::make_unique<Func>(*func_decl, scope);
	scope->define(func_decl->id->get_name(), std::move(func));
}

void Interpreter::visit(FuncCall * func_call){
	obj_ptr lhs = eval(func_call->left.get());
	ObjList args;
	for(const auto & arg : func_call->args){
		args.push_back(eval(arg.get()));
	}

	// TODO: Add check for function type
	Func * func = static_cast<Func*>(lhs.get());

	call(func, std::move(args));
}

void Interpreter::visit(InfixOp * infix_op){

}

void Interpreter::visit(IfExpression * if_expr){

}