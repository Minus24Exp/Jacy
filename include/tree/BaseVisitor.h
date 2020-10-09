#ifndef BASEVISITOR_H
#define BASEVISITOR_H

#include <vector>

struct Node;

struct Stmt;
struct ExprStmt;
struct VarDecl;
struct Block;
struct FuncDecl;
struct ReturnStmt;
struct WhileStmt;
struct ForStmt;
struct ClassDecl;
struct Import;
struct TypeDecl;

struct Expr;
struct Literal;
struct Identifier;
struct Infix;
struct Prefix;
struct Assign;
struct SetExpr;
struct GetExpr;
struct FuncCall;
struct IfExpr;
struct ListExpr;
struct GetItem;
struct SetItem;
struct DictExpr;

class BaseVisitor {
public:
    BaseVisitor() = default;
    virtual ~BaseVisitor() = default;

    // Statements //
    virtual void visit(ExprStmt * expr_stmt) = 0;
    virtual void visit(Block * block) = 0;
    virtual void visit(VarDecl * var_decl) = 0;
    virtual void visit(FuncDecl * func_decl) = 0;
    virtual void visit(ReturnStmt * return_stmt) = 0;
    virtual void visit(WhileStmt * w) = 0;
    virtual void visit(ForStmt * for_stmt) = 0;
    virtual void visit(ClassDecl * class_decl) = 0;
    virtual void visit(Import * import) = 0;
    virtual void visit(TypeDecl * type_decl) = 0;

    // Expressions //
    virtual void visit(Literal * literal) = 0;
    virtual void visit(Identifier * id) = 0;
    virtual void visit(Infix * infix) = 0;
    virtual void visit(Prefix * prefix) = 0;
    virtual void visit(Assign * assign) = 0;
    virtual void visit(SetExpr * set_expr) = 0;
    virtual void visit(GetExpr * get_expr) = 0;
    virtual void visit(FuncCall * func_call) = 0;
    virtual void visit(IfExpr * if_expr) = 0;
    virtual void visit(ListExpr * list) = 0;
    virtual void visit(GetItem * get_item) = 0;
    virtual void visit(SetItem * set_item) = 0;
    virtual void visit(DictExpr * dict) = 0;

};

#endif