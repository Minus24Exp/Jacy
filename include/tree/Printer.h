#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

#include "BaseVisitor.h"
#include "tree/nodes.h"

namespace jc::tree {
    class Printer : public BaseVisitor {
    public:
        Printer();
        ~Printer() override = default;

        void print(const StmtList & tree);

        // Statements //
        void visit(ExprStmt * expr_stmt) override;
        void visit(Block * block) override;
        void visit(VarDecl * var_decl) override;
        void visit(FuncDecl * func_decl) override;
        void visit(ReturnStmt * return_stmt) override;
        void visit(WhileStmt * w) override;
        void visit(ForStmt * for_stmt) override;
        void visit(ClassDecl * class_decl) override;
        void visit(Import * import) override;
        void visit(TypeDecl * type_decl) override;

        // Expressions //
        void visit(Literal * literal) override;
        void visit(Identifier * id) override;
        void visit(Infix * infix) override;
        void visit(Prefix * prefix) override;
        void visit(Assign * assign) override;
        void visit(SetExpr * set_expr) override;
        void visit(GetExpr * get_expr) override;
        void visit(FuncCall * func_call) override;
        void visit(IfExpr * if_expr) override;
        void visit(ListExpr * list) override;
        void visit(GetItem * get_item) override;
        void visit(SetItem * set_item) override;
        void visit(DictExpr * dict) override;
        void visit(MethodCall * method_call) override;
        void visit(SpreadExpr * spread_expr) override;

    private:
        void print_indent() const;

        size_t indent{};
    };
}

#endif