#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>
#include <iostream>

#include "BaseVisitor.h"
#include "Exception.h"
#include "tree/nodes.h"
#include "backend/Scope.h"
#include "object/objects.h"
#include "backend/Global.h"

class Interpreter : public BaseVisitor {
public:
	static Interpreter & get_instance(){
		static Interpreter instance;
		return instance;
	}

	Interpreter(const Interpreter&) = delete;
	Interpreter(Interpreter&&) = delete;
	Interpreter & operator=(const Interpreter&) = delete;
	Interpreter & operator=(Interpreter&&) = delete;

private:
	Interpreter();
	~Interpreter() = default;

public:
	void interpret(const StmtList & tree);

	void enter_scope(scope_ptr new_scope = nullptr);
	void exit_scope();
	scope_ptr get_scope() const {
		return scope;
	}

	void execute(Stmt * stmt);
	obj_ptr eval(Expr * expr);
	void execute_block(Block * block, scope_ptr new_scope = nullptr);
	void eval_assign(Infix * infix);

	void visit(ExprStmt * expr_stmt) override;
	void visit(Literal * literal) override;
	void visit(Identifier * id) override;
	void visit(VarDecl * var_decl) override;
	void visit(Block * block) override;
	void visit(FuncDecl * func_decl) override;
	void visit(FuncCall * func_call) override;
	void visit(Infix * infix) override;
	void visit(IfExpr * if_expr) override;
	void visit(While * w) override;

	void runtime_error(const std::string & msg, Node * n);

private:
	obj_ptr value;
	scope_ptr scope;
};

#endif