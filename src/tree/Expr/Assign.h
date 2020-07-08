#ifndef ASSIGN_H
#define ASSIGN_H

// Note: In Yocto assignment is an expression
// Note: `Assign` is expression for assignment with identifier only !!!
// It means, that it's only valid for `id = value`
// For other assignments there's `SetExpr` and for array will be one separate too.

struct Assign : Expr {
	id_ptr id;
	expr_ptr value;

	Assign(const Position & pos, id_ptr id, expr_ptr value)
		: Expr(pos, ExprType::Assign), id(id), value(value) {}
	virtual ~Assign() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif