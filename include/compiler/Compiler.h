#ifndef COMPILER_H
#define COMPILER_H

#include "tree/BaseVisitor.h"
#include "tree/nodes.h"
#include "compiler/opcode.h"
#include "Exception.h"
#include "compiler/Scope.h"
#include "compiler/types.h"

#include <cstring>
#include <cstdint>
#include <map>

class Compiler : public BaseVisitor {
public:
    Compiler();
    ~Compiler() override = default;

    Chunk compile(const StmtList & tree);

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

private:
    // Bytecode
    Chunk chunk;
    void emit(uint8_t byte);
    void emit(OpCode opcode);
    void emit(const uint8_t * byte_array, int size);
    void emit(uint16_t s);
    void emit(uint32_t i);
    void emit(uint64_t l);

    // Constants
    // Note: Maybe prefer custom size_t, for always 8-byte size
    std::map<long long, size_t> int_constants;
    std::map<double, size_t> float_constants;
    std::map<std::string, size_t> string_constants;

    void emit_int(long long int_val);
    void emit_float(double float_val);
    void emit_string(const std::string & string_val);
    size_t make_string(const std::string & string_val);

    // Scope
    int32_t scope_depth;
    scope_ptr scope;
    void enter_scope();
    void exit_scope();

    // Variables
    int64_t resolve_local(const scope_ptr & _scope, Identifier * id);
    int64_t resolve_upvalue(const scope_ptr & _scope, Identifier * id);
    void emit_id(Identifier * id);
    uint32_t var(Identifier * id);
    void declare_var(VarDeclKind kind, type_ptr type, Identifier * id);
    void add_local(VarDeclKind kind, type_ptr type, const std::string & name);

    // Jumps
    size_t emit_jump(OpCode jump_instr);
    void patch_jump(size_t offset);

    // Types
    type_ptr get_type(Identifier * id);

private:
    void error(const std::string & msg) {}
};

#endif // COMPILER_H
