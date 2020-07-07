#include "backend/Interpreter.h"

Interpreter::Interpreter(){
	value = nullptr;
	scope = std::make_shared<Scope>();

	Global global(*this);
	global.reg();
}

void Interpreter::interpret(const StmtList & tree){
	for(const auto & stmt : tree){
		execute(stmt.get());
	}
}

void Interpreter::enter_scope(scope_ptr new_scope){
	if(new_scope){
		scope = std::move(new_scope);
	}else{
		scope = std::make_shared<Scope>(scope);
	}
}

void Interpreter::exit_scope(){
	scope = scope->get_parent();
}

void Interpreter::execute(Stmt * stmt){
	stmt->accept(*this);
}

obj_ptr Interpreter::eval(Expr * expr){
	expr->accept(*this);
	return std::move(value);
}

void Interpreter::execute_block(Block * block, scope_ptr new_scope){
	enter_scope(new_scope);
	for(const auto & stmt : block->stmts){
		execute(stmt.get());
	}
	exit_scope();
}

void Interpreter::eval_assign(Infix * infix){
	// Assignment is right-associative
	// So evaluate rhs
	obj_ptr rhs = eval(infix->right.get());

	switch(infix->left->type){
		case ExprType::Id:{
			const auto id = std::static_pointer_cast<Identifier>(infix->left);
			int assign = scope->assign(id->get_name(), std::move(rhs));

			if(assign == 0){
				runtime_error(id->get_name() +" is not defined", id.get());
			}else if(assign == -1){
				runtime_error("Unable to reassign val "+ id->get_name(), id.get());
			}
			break;
		}
		default:{
			runtime_error("Invalid left-hand side in assignment", infix->left.get());
		}
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
	obj_ptr obj = scope->get(id->get_name());
	if(!obj){
		runtime_error(id->get_name() + " is not defined", id);
		return;
	}
	value = obj;
}

void Interpreter::visit(VarDecl * var_decl){
	// Important difference between nullptr and null_obj
	// if variable is defined, but not assigned then
	// it's nullptr, and if it's `val` then it can be assigned
	// somewhere after

	value = nullptr;

	if(var_decl->assign_expr){
		value = eval(var_decl->assign_expr.get());
	}

	// TODO: Think about cloning...
	LocalDeclType decl_type = LocalDeclType::Var;
	if(var_decl->decl == VarDeclType::Val){
		decl_type = LocalDeclType::Val;
	}

	bool defined = scope->define(var_decl->id->get_name(), {decl_type, value});
	if(!defined){
		// If tried to redefine
		runtime_error("Redifinition of variable "+ var_decl->id->get_name(), var_decl);
		return;
	}

	value = nullptr;
}

void Interpreter::visit(Block * block){
	execute_block(block);
}

void Interpreter::visit(FuncDecl * func_decl){
	std::string func_name = func_decl->id->get_name();

	Params params;
	for(auto & p : func_decl->params){
		obj_ptr default_val = nullptr; 
		if(p.default_val){
			default_val = eval(p.default_val.get());
		}
		params.push_back({p.id->get_name(), default_val});
	}

	obj_ptr func = std::make_shared<Func>(scope, func_name, params, std::move(func_decl->body));

	bool defined = scope->define(func_name, {LocalDeclType::Val, func});
	if(!defined){
		runtime_error("Redifinition of function "+ func_name, func_decl);
	}
}

void Interpreter::visit(FuncCall * func_call){
	obj_ptr lhs = eval(func_call->left.get());

	ObjList args;
	for(const auto & arg : func_call->args){
		args.push_back(eval(arg.get()));
	}

	// TODO: Add check for function type
	if(lhs->type != ObjectType::Callable){
		runtime_error("Invalid left-hand side in function call", func_call->left.get());
		return;
	}

	Callable * callable = static_cast<Callable*>(lhs.get());

	// Compare args and check for errors
	const auto required_argc_s = std::to_string(callable->get_required_argc());
	const auto max_argc_s = std::to_string(callable->get_max_argc());
	const auto given_argc_s = std::to_string(args.size());
	switch(callable->cmp_args(args)){
		case CmpArgsResult::TooFew:{
			runtime_error("Too few arguments in function call "+ callable->to_string()
						 +" (at least " + required_argc_s +" expected, "
						 + given_argc_s +" given)", func_call);
			break;
		}
		case CmpArgsResult::TooMany:{
			runtime_error("Too many arguments in function call "+ callable->to_string()
						 +" (maximum "+ max_argc_s +" expected, "
						 + given_argc_s +" given)", func_call);
			break;
		}
	}

	// TODO: !!! Add exception handling for call
	// It will be really useful for NativeFunc

	value = callable->call(*this, std::move(args));
	
	if(!value){
		// Note: This is just a helper for built-in functions
		// They can return nullptr, and then here it will be converted to Null.
		// But, nullptr does not equal to Null
		value = null_obj;
	}
}

void Interpreter::visit(Infix * infix){
	// Assignment is infix too.
	// I don't want to separate it from other operators on parsing level,
	// because of convenient precedence parsing,
	// so I do it here.
	// Assignment works pretty different from other operators,
	// for simple `id = value` assignment there's one way,
	// for `obj.field = value` there's different way.
	// and so on.

	if(infix->op.op() == Operator::Assign){
		eval_assign(infix);
		return;
	}
}

void Interpreter::visit(IfExpr * if_expr){
	auto cond = eval(if_expr->cond.get());

	if(cond->truthy()){
		execute_block(if_expr->if_branch.get());
	}else if(if_expr->else_branch){
		execute_block(if_expr->else_branch.get());
	}
}

void Interpreter::visit(While * w){
	while(eval(w->cond.get())->truthy()){
		execute_block(w->body.get());
	}
}

void Interpreter::runtime_error(const std::string & msg, Node * n){
	throw RuntimeException(msg, n);
}