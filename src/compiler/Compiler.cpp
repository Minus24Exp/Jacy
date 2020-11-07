#include "compiler/Compiler.h"

Compiler::Compiler() : scope_depth(0) {}

Chunk Compiler::compile(const StmtList & tree) {
    enter_scope();

    for (const auto & stmt : tree) {
        stmt->accept(*this);
    }

    exit_scope();
    return chunk;
}

////////////////
// Statements //
////////////////
void Compiler::visit(ExprStmt * expr_stmt) {
    expr_stmt->expr->accept(*this);
}

void Compiler::visit(Block * expr_stmt) {

}

void Compiler::visit(VarDecl * var_decl) {
    VarDeclKind kind = var_decl->kind;

    // TODO: Add real types (now any)
    type_ptr type = any_t;

    if (scope_depth == 0) {
        // Define global
        uint32_t global = make_string(var_decl->id->get_name());
        emit(OpCode::DefineGlobal);
        emit(static_cast<uint32_t>(global));
        if (var_decl->assign_expr) {
            var_decl->assign_expr->accept(*this);
            emit(OpCode::StoreGlobal);
        }
    } else {
        declare_var(kind, type, var_decl->id.get());
        if (var_decl->assign_expr) {
            var_decl->assign_expr->accept(*this);
            emit(OpCode::StoreLocal);
            emit(scope->locals.size() - 1);
            scope->locals.back().depth = scope_depth;
        }
    }
}

void Compiler::visit(FuncDecl * func_decl) {
}

void Compiler::visit(ReturnStmt * expr_stmt) {

}

void Compiler::visit(WhileStmt * expr_stmt) {

}

void Compiler::visit(ForStmt * expr_stmt) {

}

void Compiler::visit(ClassDecl * class_decl) {
}

void Compiler::visit(Import * expr_stmt) {

}

void Compiler::visit(TypeDecl * expr_stmt) {

}

/////////////////
// Expressions //
/////////////////
void Compiler::visit(Literal * literal) {
    switch (literal->token.type) {
        case TokenType::Null: {
            emit(OpCode::NullConst);
        } break;
        case TokenType::True: {
            emit(OpCode::TrueConst);
        } break;
        case TokenType::False: {
            emit(OpCode::FalseConst);
        } break;
        case TokenType::Int: {
            // TODO: Add conversion exception handling
            long long int_val = std::stoll(literal->token.val);
            emit_int(int_val);
//            for (int i = 0; i < sizeof(long long); i++) {
//                emit((uint8_t)((int_val >> (i * 8)) & 0xFF));
//            }
        } break;
        case TokenType::Float: {
            // TODO: Add conversion exception handling
            double float_val = std::stod(literal->token.val);
            emit_float(float_val);
//            emit(reinterpret_cast<uint8_t*>(&double_val), sizeof(double));
        } break;
        case TokenType::String: {
            const auto & string_val = literal->token.val;
            emit_string(string_val);
//            emit(static_cast<uint64_t>(str.size()));
//            // TODO: Add encoding support
//            for (const auto & c : str) {
//                emit(static_cast<uint8_t>(c));
//            }
        } break;
        default: {
            throw DevError("Unexpected type of literal token");
        }
    }
}

void Compiler::visit(Identifier * id) {
    emit_id(id);
}

void Compiler::visit(Infix * expr_stmt) {

}

void Compiler::visit(Prefix * expr_stmt) {

}

void Compiler::visit(Assign * assign) {
    assign->value->accept(*this);
    size_t operand = resolve_local(scope, assign->id.get());
    OpCode opcode;
    if (operand == -1) {
        operand = make_string(assign->id->get_name());
        opcode = OpCode::StoreGlobal;
    } else {
        opcode = OpCode::StoreLocal;
    }
    emit(opcode);
    emit(operand);
}

void Compiler::visit(SetExpr * set_expr) {
    set_expr->left->accept(*this);

    size_t name = make_string(set_expr->id->get_name());
    set_expr->value->accept(*this);
    emit(OpCode::SetProperty);
    emit(name);
}

void Compiler::visit(GetExpr * get_expr) {
    get_expr->left->accept(*this);
    size_t name = make_string(get_expr->id->get_name());
    emit(OpCode::GetProperty);
    emit(name);
}

void Compiler::visit(FuncCall * func_call) {
    size_t arg_count = 0;
    for (const auto & arg : func_call->args) {
        arg->accept(*this);
        arg_count++;
    }
    func_call->left->accept(*this);
    emit(OpCode::Call);
    emit(arg_count);
}

void Compiler::visit(IfExpr * if_expr) {
    if_expr->cond->accept(*this);

    size_t then_jump = emit_jump(OpCode::JumpFalse);
    emit(OpCode::Pop);
    if_expr->if_branch->accept(*this);

    size_t else_jump = emit_jump(OpCode::Jump);
    patch_jump(then_jump);
    emit(OpCode::Pop);

    if (if_expr->else_branch) {
        if_expr->else_branch->accept(*this);
    }

    patch_jump(else_jump);
}

void Compiler::visit(ListExpr * list_expr) {

}

void Compiler::visit(GetItem * expr_stmt) {

}

void Compiler::visit(SetItem * expr_stmt) {

}

void Compiler::visit(DictExpr * expr_stmt) {

}

//////////////
// Bytecode //
//////////////
void Compiler::emit(uint8_t byte) {
    chunk.code.push_back(byte);
}

void Compiler::emit(OpCode opcode) {
    emit(static_cast<uint8_t>(opcode));
}

void Compiler::emit(const uint8_t * byte_array, int size) {
    for (std::size_t i = 0; i < size; i++) {
        emit(byte_array[i]);
    }
}

void Compiler::emit(uint16_t s) {
    emit(reinterpret_cast<uint8_t*>(&s), 2);
}

void Compiler::emit(uint32_t i) {
    emit(reinterpret_cast<uint8_t*>(&i), 4);
}

void Compiler::emit(uint64_t l) {
    emit(reinterpret_cast<uint8_t*>(&l), 8);
}

///////////////
// Constants //
///////////////
void Compiler::emit_int(long long int_val) {
    emit(OpCode::IntConst);
    const auto & found = int_constants.find(int_val);
    if (found != int_constants.end()) {
        emit(found->second);
        return;
    }
    chunk.constants.push_back(IntConstant{int_val});
    int_constants[int_val] = chunk.constants.size() - 1;
    emit(chunk.constants.size() - 1);
}

void Compiler::emit_float(double float_val) {
    emit(OpCode::FloatConst);
    const auto & found = float_constants.find(float_val);
    if (found != float_constants.end()) {
        emit(found->second);
        return;
    }
    chunk.constants.push_back(FloatConstant{float_val});
    float_constants[float_val] = chunk.constants.size() - 1;
    emit(chunk.constants.size() - 1);
}

void Compiler::emit_string(const std::string & string_val) {
    emit(OpCode::StringConst);
    emit(make_string(string_val));
}

size_t Compiler::make_string(const std::string & string_val) {
    const auto & found = string_constants.find(string_val);
    if (found != string_constants.end()) {
        return found->second;
    }
    chunk.constants.push_back(StringConstant{string_val});
    string_constants[string_val] = chunk.constants.size() - 1;
    return chunk.constants.size() - 1;
}

///////////
// Scope //
///////////
void Compiler::enter_scope() {
    scope_depth++;
    scope = std::make_shared<Scope>(scope);
}

void Compiler::exit_scope() {
    scope_depth--;
    if (!scope->parent) {
        throw DevError("Attempt to exit global scope");
    }
    scope = scope->parent;
}

///////////////
// Variables //
///////////////
int64_t Compiler::resolve_local(const scope_ptr & _scope, Identifier * id) {
    for (size_t i = _scope->locals.size() - 1; i >= 0; i--) {
        const auto & local = _scope->locals[i];
        if (local.name == id->get_name()) {
            if (local.depth == -1) {
                error(id->get_name() + " is not defined");
            }
            return i;
        }
    }

    return -1;
}

int64_t Compiler::resolve_upvalue(const scope_ptr & _scope, Identifier * id) {
    if (!_scope->parent) {
        return -1;
    }

    int64_t local = resolve_local(_scope, id);
    if (local != -1) {
        _scope->parent->locals[local].is_captured = true;
//        return add_upvalue(_scope, static_cast<uint32_t>(local), true);
    }

    int64_t upvalue = resolve_upvalue(_scope->parent, id);
    if (upvalue != -1) {
//        return add_upvalue(_scope, static_cast<uint32_t>(upvalue), false);
    }

    return -1;
}

void Compiler::emit_id(Identifier * id) {
    size_t operand = resolve_local(scope, id);
    OpCode opcode;
    if (operand == -1) {
        operand = make_string(id->get_name());
        opcode = OpCode::LoadGlobal;
    } else {
        opcode = OpCode::LoadLocal;
    }
    emit(opcode);
    emit(operand);
}

uint32_t Compiler::var(Identifier * id) {
//    declare_var(id);
//
//    if (scope_depth > 0) {
//        return 0;
//    }
//
//    return make_string(id->get_name());
}

void Compiler::declare_var(VarDeclKind kind, type_ptr type, Identifier * id) {
    if (scope_depth == 0) {
        return;
    }

    for (size_t i = scope->locals.size() - 1; i >= 0; i--) {
        const auto & local = scope->locals[i];
        if (local.depth != -1 && local.depth < scope_depth) {
            break;
        }
        if (id->get_name() == local.name) {
            error(id->get_name() + " has been already declared in this scope");
        }
    }

    add_local(kind, type, id->get_name());
}

void Compiler::add_local(VarDeclKind kind, type_ptr type, const std::string & name) {
    if (scope->locals.size() == UINT32_MAX) {
        error("Unable to handle too many locals");
        return;
    }

    scope->locals.emplace_back(kind, type, name);
}

///////////
// Jumps //
///////////
size_t Compiler::emit_jump(OpCode jump_instr) {
    emit(jump_instr);
    for (int i = 0; i < jump_space; i++) {
        emit(0xFFu);
    }
    return chunk.code.size() - jump_space;
}

void Compiler::patch_jump(size_t offset) {
    size_t jump = chunk.code.size() - offset - jump_space;

    // Check jump offset if it's bigger than jump_size type size

    for (int i = jump_space; i >= 0; i--) {
        chunk.code[offset + jump_space - i] = (jump >> (i * 8u)) & 0xFFu;
    }
}

///////////
// Types //
///////////
type_ptr Compiler::get_type(Identifier * id) {
    scope_ptr _scope = scope;

    while (_scope) {
        for (const auto & local : _scope->locals) {
            if (local.name == id->get_name()) {
                return local.type;
            }
        }
        _scope = _scope->parent;
    }

    return nullptr;
}
