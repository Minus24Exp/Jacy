#include "YoctoVisitor.h"

YoctoVisitor::YoctoVisitor(){
	// Enter root scope
	enter_scope();

	// Register all built-ins including classes and functions
	register_builtins(*this);
}

void YoctoVisitor::enter_scope(Scope * new_scope){
	if(new_scope){
		new_scope->set_parent(scope);
		scope = new_scope;
	}else{
		scope = new Scope(scope);
	}
}

void YoctoVisitor::exit_scope(){
	Scope * parent = scope->get_parent();
	delete scope;
	scope = parent;
}

void YoctoVisitor::eval(const ParseTree & tree){
	for(Node * stmt : tree){
		stmt->accept(*this);
	}
	exit_scope();
}

void YoctoVisitor::visit(ExprStmt & expr_stmt){
	expr_stmt.expr.accept(*this);
}

void YoctoVisitor::visit(Literal & literal){
	switch(literal.token.type){
		case TokenType::Int:{
			value = new Int(literal.token.Int());
			break;
		}
		case TokenType::Float:{
			value = new Float(literal.token.Float());
			break;
		}
		case TokenType::Str:{
			value = new String(literal.token.String());
			break;
		}
		case TokenType::Bool:{
			if(literal.token.Bool()){
				value = new TrueClass();
			}else{
				value = new FalseClass();
			}
			break;
		}
		default:{
			throw YoctoException("Unexpected literal type");		
		}
	}
}

void YoctoVisitor::visit(Identifier & id){
	value = scope->lookup(id.get_name());
}

void YoctoVisitor::visit(VarDecl & var_decl){
	// TODO: Add var/val difference
	
	if(var_decl.assign_expr){
		var_decl.assign_expr->accept(*this);
		scope->define(var_decl.id.get_name(), value);
	}else{
		// By default define as null
		scope->define(var_decl.id.get_name(), NullClass)
	}
}

void YoctoVisitor::visit(Block & block){
	enter_scope();
	for(Statement * stmt : block.stmts){
		stmt->accept(*this);
	}
	exit_scope();
}

void YoctoVisitor::visit(FuncDecl & func_decl){
	scope->define(func_decl.id.get_name(), new Method(func_decl));
}

void YoctoVisitor::visit(FuncCall & func_call){
	// Eval left-hand side of function call (it can be not only an Identifier)
	func_call.left.accept(*this);

	Callable * func = dynamic_cast<Callable*>(value);

	if(!func){
		throw YoctoException("Invalid left-hand side in call");
	}

	Args args;
	for(Expression * expr : func_call.args){
		expr->accept(*this);
		args.push_back(value);
	}

	func->call(*this, args);
}

void YoctoVisitor::visit(InfixOp & infix_op){
	// TODO: Add assignment

	infix_op.left.accept(*this);

	std::string magic;

	switch(infix_op.op.op()){
		case Operator::Add: magic = "__add"; break;
		case Operator::Sub: magic = "__sub"; break;
		case Operator::Mul: magic = "__mul"; break;
		case Operator::Div: magic = "__div"; break;
		case Operator::Mod: magic = "__mod"; break;
		default: throw YoctoException("Unexpected infix operator");
	}

	Callable * magic_method = dynamic_cast<Callable*>(value->find_field(magic));
	
	if(!magic_method){
		throw YoctoException("Unable to use `" + op_to_str(infix_op.op.op()) + "` operator");
	}

	infix_op.right.accept(*this);

	magic_method->call(*this, { value });
}

void YoctoVisitor::visit(IfExpression & if_expr){
}