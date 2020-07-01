#ifndef YOCTOVISITOR_H
#define YOCTOVISITOR_H

#include "BaseVisitor.h"
#include "Token.h"
#include "tree/nodes.h"
#include "Object/Int.h"
#include "Object/Float.h"
#include "Object/String.h"
#include "Object/Method.h"
#include "Object/NativeMethod.h"
#include "Object/NullClass.h"
#include "Object/FalseClass.h"
#include "Object/TrueClass.h"
#include "backend/Scope.h"
#include "backend/register_builtins.c"

#include <iostream>

class YoctoVisitor : public BaseVisitor {
public:
	YoctoVisitor();
	virtual ~YoctoVisitor() = default;

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