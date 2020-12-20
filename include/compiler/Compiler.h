#ifndef COMPILER_H
#define COMPILER_H

#include "tree/BaseVisitor.h"
#include "tree/nodes.h"
#include "bytecode/opcode.h"
#include "Exception.h"
#include "compiler/Scope.h"
#include "compiler/type.h"
#include "../../lib/globals.h"
#include "common/Logger.h"

#include <cstring>
#include <cstdint>
#include <map>

namespace jc::compiler {
    class Compiler : public tree::BaseVisitor {
    public:
        Compiler();
        ~Compiler() override = default;

        bytecode::Chunk compile(const tree::StmtList & tree);

        // Statements //
        void visit(tree::ExprStmt * expr_stmt) override;
        void visit(tree::Block * block) override;
        void visit(tree::VarDecl * var_decl) override;
        void visit(tree::FuncDecl * func_decl) override;
        void visit(tree::ReturnStmt * return_stmt) override;
        void visit(tree::WhileStmt * w) override;
        void visit(tree::ForStmt * for_stmt) override;
        void visit(tree::ClassDecl * class_decl) override;
        void visit(tree::Import * import) override;
        void visit(tree::TypeDecl * type_decl) override;

        // Expressions //
        void visit(tree::Grouping * grouping) override;
        void visit(tree::Literal * literal) override;
        void visit(tree::Identifier * id) override;
        void visit(tree::Infix * infix) override;
        void visit(tree::Prefix * prefix) override;
        void visit(tree::Assign * assign) override;
        void visit(tree::SetExpr * set_expr) override;
        void visit(tree::GetExpr * get_expr) override;
        void visit(tree::FuncCall * func_call) override;
        void visit(tree::MethodCall * method_call) override;
        void visit(tree::IfExpr * if_expr) override;
        void visit(tree::ListExpr * list) override;
        void visit(tree::GetItem * get_item) override;
        void visit(tree::SetItem * set_item) override;
        void visit(tree::DictExpr * dict) override;
        void visit(tree::SpreadExpr * spread_expr) override;

        // Types //
        void visit(tree::IdType * id_type) override;
        void visit(tree::ListType * list_type) override;
        void visit(tree::DictType * dict_type) override;
        void visit(tree::GenericType * generic_type) override;
        void visit(tree::UnionType * union_type) override;
        void visit(tree::FuncType * func_type) override;

    private:
        // Bytecode //
        bytecode::Chunk chunk;
        void emit(uint8_t byte);
        void emit(bytecode::OpCode opcode);
        void emit(const uint8_t * byte_array, int size);
        void emit(uint16_t s);
        void emit(uint32_t i);
        void emit(uint64_t l);

        // Constants //
        std::map<long long, uint32_t> int_constants;
        std::map<double, uint32_t> float_constants;
        std::map<std::string, uint32_t> string_constants;

        uint32_t add_const(const bytecode::constant_ptr & constant);
        void emit_int(long long int_val);
        void emit_float(double float_val);
        void emit_string(const std::string & string_val);
        uint32_t make_string(const std::string & string_val);

        // Scope //
        uint32_t scope_depth;
        scope_ptr scope;
        void enter_scope();
        scope_ptr exit_scope();
        std::shared_ptr<bytecode::FuncConstant> current_function;

        std::map<std::string, var_ptr> globals;

        // Variables //
        int64_t resolve_local(const scope_ptr & _scope, tree::Identifier * id);
        int64_t resolve_upvalue(const scope_ptr & _scope, tree::Identifier * id);
        void emit_id(tree::Identifier * id);
        void declare_var(VarDeclKind kind, type_ptr type, tree::Identifier * id);
        void add_local(VarDeclKind kind, type_ptr type, const std::string & name);
        uint32_t add_upvalue(const scope_ptr & scope, uint32_t offset, bool is_local);

        // Jumps //
        int32_t emit_jump(bytecode::OpCode jump_instr);
        void patch_jump(int32_t offset);

        // Type checking //
        // TODO: ? Maybe add reset_type and set_type funcs for explicitly
        type_ptr last_type{nullptr};
        type_ptr resolve_type(tree::IdType * id);

        // Errors //
        static void error(const std::string & msg, const Position & pos);
        static void undefined_entity();

        // DEBUG //
    public:
        common::CompilerOptions options;
        common::Logger log;
    };
}

#endif // COMPILER_H
