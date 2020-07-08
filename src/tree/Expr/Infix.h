#ifndef INFIX_H
#define INFIX_H

#include "tree/Expr/Expr.h"

enum class InfixPrec {
	Zero,
	Assign,
	// OR,
	// AND,
	// BIT_OR,
	// BIT_AND,
	// EQ,
	// CMP,
	// SH,
	// RANGE,
	Term,
	Factor,
	// EXP,
	MemberAccess,
	None
};

inline int get_infix_prec(const Operator & op){
	InfixPrec prec;
	switch(op){
		case Operator::Assign:{
			prec = InfixPrec::Assign;
			break;
		}
		case Operator::Add:
		case Operator::Sub:{
			prec = InfixPrec::Term;
			break;
		}
		case Operator::Mul:
		case Operator::Div:
		case Operator::Mod:{
			prec = InfixPrec::Factor;
			break;
		}
		case Operator::Dot:{
			prec = InfixPrec::MemberAccess;
			break;
		}
		default:{
			prec = InfixPrec::None;
		}
	}

	return static_cast<int>(prec);
}

struct Infix : Expr {
	expr_ptr left;
	Token op;
	expr_ptr right;

	Infix(const Position & pos, expr_ptr left, const Token & op, expr_ptr right)
		   : Expr(pos, ExprType::Infix), left(left), op(op), right(right) {}

	virtual ~Infix() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif