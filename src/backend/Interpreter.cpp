#include "backend/Interpreter.h"

Interpreter::Interpreter(){
	value = nullptr;
	scope = std::make_shared<Scope>();

	scope->define("print", make_nf(scope, "print", { {"o"} }, [this](NFArgs && args) -> obj_ptr {
		std::cout << args["o"]->to_string() << std::endl;

		return nullptr;
	}));
}

void Interpreter::interpret(const StmtList & tree){
	for(const auto & stmt : tree){
		execute(stmt.get());
	}
}

void Interpreter::enter_scope(scope_ptr sub_scope){
	if(sub_scope){
		sub_scope->set_parent(scope);
		scope = sub_scope;
	}else{
		scope = std::make_shared<Scope>(scope);
	}
}

void Interpreter::exit_scope(){
	scope = scope->get_parent();
}

void Interpreter::execute(Statement * stmt){
	stmt->accept(*this);
}

obj_ptr Interpreter::eval(Expression * expr){
	expr->accept(*this);
	return std::move(value);
}

void Interpreter::execute_block(Block * block, scope_ptr sub_scope){
	enter_scope(sub_scope);
	for(const auto & stmt : block->stmts){
		execute(stmt.get());
	}
	exit_scope();
}

void Interpreter::visit(ExprStmt * expr_stmt){
	value = eval(expr_stmt->expr.get());
}

void Interpreter::visit(Literal * literal){
	switch(literal->token.type){
		case TokenType::Null:{
			value.reset(new Null(scope));
			break;
		}
		case TokenType::Bool:{
			value.reset(new Bool(scope, literal->token.Bool()));
			break;
		}
		case TokenType::Int:{
			value.reset(new Int(scope, literal->token.Int()));
			break;
		}
		case TokenType::Float:{
			value.reset(new Float(scope, literal->token.Float()));
			break;
		}
		case TokenType::Str:{
			value.reset(new String(scope, literal->token.String()));
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
	if(var_decl->assign_expr){
		value = eval(var_decl->assign_expr.get());
		scope->define(var_decl->id->get_name(), value->clone());
	}else{
		scope->define(var_decl->id->get_name(), std::make_unique<Null>(scope));
	}

	value = nullptr;
}

void Interpreter::visit(Block * block){
	execute_block(block);
}

void Interpreter::visit(FuncDecl * func_decl){
	obj_ptr func = std::make_unique<Func>(scope, *func_decl);
	scope->define(func_decl->id->get_name(), std::move(func));
}

void Interpreter::visit(FuncCall * func_call){
	obj_ptr lhs = eval(func_call->left.get());
	ObjList args;
	for(const auto & arg : func_call->args){
		args.push_back(eval(arg.get()));
	}

	// TODO: Add check for function type
	if(lhs->type != ObjectType::Callable){
		throw YoctoException("Invalid left-hand side in function call");
		return;
	}

	Callable * callable = static_cast<Callable*>(lhs.get());

	if(!callable->cmp_args(args)){
		throw YoctoException("Invalid arguments");
	}

	// TODO: !!! Add exception handling for call
	// It will be really useful for NativeFunc

	value = callable->call(*this, std::move(args));
	
	if(!value){
		// Note: This is just a helper for built-in functions
		// They can return nullptr, and then here it will be converted to Null.
		// But, nullptr does not equal to Null
		value = make_null(scope);
	}
}

void Interpreter::visit(InfixOp * infix_op){
}

void Interpreter::visit(IfExpression * if_expr){

}