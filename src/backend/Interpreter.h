#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "BaseVisitor.h"
#include "Token.h"
#include "tree/nodes.h"
#include "backend/Scope.h"
#include "backend/register_builtins.c"

#include "Object/Object.h"
#include "Object/NativeMethod.h"
#include "Object/Method.h"

#include <iostream>

class Interpreter : public BaseVisitor {
public:
	Interpreter();
	virtual ~Interpreter() = default;

	// Scope * get_scope() const {
	// 	// Used to define built-ins before compilation outside of visitor
	// 	return scope;
	// }

	void eval(const ParseTree & tree);

	void visit(ExprStmt & expr_stmt) override;
	void visit(Literal & literal) override;
	void visit(Identifier & id) override;
	void visit(VarDecl & var_decl) override;
	void visit(Block & block) override;
	void visit(FuncDecl & func_decl) override;
	void visit(FuncCall & func_call) override;
	void visit(InfixOp & infix_op) override;
	void visit(IfExpression & if_expr) override;

	Object * get_value() const {
		return value;
	}

private:
	Object * value;

	// Scope
	Scope * scope;
	void enter_scope(Scope * new_scope = nullptr);
	void exit_scope();

};

#endif