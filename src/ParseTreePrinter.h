#ifndef PARSETREEPRINTER_H
#define PARSETREEPRINTER_H

#include <iostream>

#include "BaseVisitor.h"

class ParseTreePrinter : public BaseVisitor {
public:
	ParseTreePrinter();
	virtual ~ParseTreePrinter() = default;

	void print(const ParseTree & tree);
	
	void visit(ExprStmt & expr_stmt) override;
	void visit(Literal & literal) override;
	void visit(Identifier & id) override;
	void visit(VarDecl & var_decl) override;
	void visit(Block & block) override;
	void visit(FuncDecl & func_decl) override;
	void visit(FuncCall & func_call) override;
	void visit(InfixOp & infix_op) override;
	void visit(IfExpression & if_expr) override;

private:
	void print_indent();

	size_t indent;
};

#endif