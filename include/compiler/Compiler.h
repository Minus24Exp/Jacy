#ifndef COMPILER_H
#define COMPILER_H

#include <cstring>
#include "tree/nodes.h"
#include "tree/BaseVisitor.h"
#include "compiler/opcode.h"
#include "vm/Value.h"
#include "object/objects.h"
#include "Exception.h"

class Compiler : public BaseVisitor {
public:
    Compiler();
    virtual ~Compiler() = default;

    Chunk compile(const StmtList & tree);

private:
    int scope_depth;
    std::vector<Local> locals;
    std::size_t resolve_local(std::string name);
    // void addConstant(Value value);

    Chunk chunk;

    void emit(uint8_t byte);
    void emit(OpCode opcode);
    void emit(const uint8_t * byte_array, int size);
    void emit(uint16_t s);
    void emit(uint32_t i);
    void emit(uint64_t l);

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

    void error(const std::string & msg);
};

#endif
