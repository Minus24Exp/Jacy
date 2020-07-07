#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

#include "BaseVisitor.h"
#include "tree/nodes.h"

class Printer : public BaseVisitor {
public:
	Printer();
	virtual ~Printer() = default;

	void print(const StmtList & tree);
	
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

private:
	void print_indent();

	size_t indent;
};

#endif