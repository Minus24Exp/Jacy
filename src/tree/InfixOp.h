#ifndef INFIXOP_H
#define INFIXOP_H

#include "tree/Node.h"

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
	// ACCESS,
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
		default:{
			prec = InfixPrec::None;
		}
	}

	return static_cast<int>(prec);
}

struct InfixOp : Expression {
	expr_ptr left;
	Token op;
	expr_ptr right;

	InfixOp(expr_ptr left, const Token & op, expr_ptr right) : left(left), op(op), right(right) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(this);
	}
};

#endif