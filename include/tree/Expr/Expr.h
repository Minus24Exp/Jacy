#ifndef EXPR_H
#define EXPR_H

#include "tree/Node.h"

struct Expr;
using expr_ptr = std::shared_ptr<Expr>;
using ExprList = std::vector<expr_ptr>;

enum class ExprType {
    Literal,
    Id,
    Infix,
    Prefix,
    Assign,
    Set,
    Get,
    Call,
    If,
    GetItem,
    SetItem,
    List,
    Dict
};

struct Expr : Node {
    Expr(const Position & pos, ExprType type) : Node(pos), type(type) {}
    ~Expr() override = default;

    void accept(BaseVisitor & visitor) override = 0;

    ExprType type;
};

#endif