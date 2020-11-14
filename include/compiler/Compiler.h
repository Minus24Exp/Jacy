#ifndef COMPILER_H
#define COMPILER_H

#include "tree/BaseVisitor.h"
#include "tree/nodes.h"
#include "compiler/opcode.h"
#include "Exception.h"
#include "compiler/Scope.h"
#include "compiler/types.h"
#include "vm/globals.h"
#include "common/Logger.h"

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
    std::map<long long, uint64_t> int_constants;
    std::map<double, uint64_t> float_constants;
    std::map<std::string, uint64_t> string_constants;

    void emit_int(long long int_val);
    void emit_float(double float_val);
    void emit_string(const std::string & string_val);
    uint64_t make_string(const std::string & string_val);

    // Scope
    uint64_t scope_depth;
    scope_ptr scope;
    void enter_scope();
    void exit_scope();

    // Variables
    uint64_t resolve_local(const scope_ptr & _scope, Identifier * id);
    uint64_t resolve_upvalue(const scope_ptr & _scope, Identifier * id);
    void emit_id(Identifier * id);
    void declare_var(VarDeclKind kind, type_ptr type, Identifier * id);
    void add_local(VarDeclKind kind, type_ptr type, const std::string & name);

    // Jumps
    int64_t emit_jump(OpCode jump_instr);
    void patch_jump(int64_t offset);

    // Type checking
    // TODO: Maybe add reset_type and set_type funcs for explicity
    type_ptr last_type{nullptr};
    std::map<std::string, var_ptr> globals;
    type_ptr resolve_type(Identifier * id);

    // Errors //
    static void error(const std::string & msg, const Position & pos);
    static void undefined_entity();

    // DEBUG //
private:
    CompilerOptions options;
    Logger log;
};

#endif // COMPILER_H
