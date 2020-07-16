#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>

#include "tree/BaseVisitor.h"
#include "backend/Scope.h"
#include "backend/Global.h"
#include "tree/nodes.h"
#include "object/objects.h"

// ReturnValue is used to catch return statement
// It has to be separate type to determine it correctly in catch
struct ReturnValue {
    obj_ptr value;
    // We need to track return value position
    // to catch errors for return statement outside of function
    Position pos;
};

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

    obj_ptr get_value() const {
        return value;
    }

    void execute(Stmt * stmt);
    obj_ptr eval(Expr * expr);
    void execute_block(Block * block, scope_ptr new_scope = nullptr);
    
    // Statements //
    void visit(ExprStmt * expr_stmt) override;
    void visit(Block * block) override;
    void visit(VarDecl * var_decl) override;
    void visit(FuncDecl * func_decl) override;
    void visit(ReturnStmt * return_stmt) override;
    void visit(WhileStmt * w) override;
    void visit(ClassDecl * class_decl) override;

    // Expressions //
    void visit(Literal * literal) override;
    void visit(Identifier * id) override;
    void visit(Infix * infix) override;
    void visit(Prefix * prefix) override;
    void visit(Postfix * postfix) override;
    void visit(Assign * assign) override;
    void visit(SetExpr * set_expr) override;
    void visit(GetExpr * get_expr) override;
    void visit(FuncCall * func_call) override;
    void visit(IfExpr * if_expr) override;
    void visit(ArrayExpr * array) override;
    void visit(GetItem * get_item) override;
    void visit(SetItem * set_item) override;

    // Errors //
    void runtime_error(const std::string & msg, const Position & pos);
    void runtime_error(const std::string & msg, Node * n);

private:
    obj_ptr value;
    scope_ptr scope;
};

#endif