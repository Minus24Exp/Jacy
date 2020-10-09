#ifndef COMPILER_H
#define COMPILER_H

#include <cstring>
#include <cstdint>
#include "tree/nodes.h"
#include "tree/BaseVisitor.h"
#include "compiler/opcode.h"
#include "vm/Value.h"
#include "Exception.h"
#include "object/objects.h"
#include "compiler/Scope.h"

class Compiler : public BaseVisitor {
public:
    Compiler();
    ~Compiler() override = default;

    scope_ptr compile(const StmtList & tree);

private:
    // Scope info
    int scope_depth;
    scope_ptr current_scope;
    // module_ptr module;
    func_ptr func;
    class_ptr _class;

    // Chunk operators
    uint64_t add_const(const Value & value);
    uint64_t make_const(const Value & value);
    void emit_const(const Value & value);
    uint64_t id_const(Identifier * id);
    void declare_var(Identifier * id, VarDeclKind kind);
    void add_local(Identifier * id, VarDeclKind kind);
    void mark_inited();
    void emit_id(Identifier * id);
    void define_var(uint64_t global);
    uint64_t compile_var(Identifier * id, VarDeclKind kind);

    uint64_t resolve_local(const scope_ptr & scope, std::string name);
    uint64_t resolve_upvalue(const scope_ptr & scope, std::string name);
    uint64_t add_upvalue(const scope_ptr & scope, uint64_t index, bool is_local);

    void enter_scope();
    void exit_scope();

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
