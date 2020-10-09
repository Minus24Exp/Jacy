#include "compiler/Compiler.h"

Compiler::Compiler() : scope_depth(0) {
    // Debug print function
    // locals.emplace(locals.begin() + print_offset, Local{0, "print", VarDeclKind::Val});
}

scope_ptr Compiler::compile(const StmtList & tree) {
    enter_scope();
    
    for (const auto & stmt : tree) {
        stmt->accept(*this);
    }

    scope_ptr global = current_scope;
    exit_scope();
    return global;
}

uint64_t Compiler::add_const(const Value & value) {
    current_scope->chunk.constants.push_back(value);
}

uint64_t Compiler::make_const(const Value & value) {
    uint64_t index = add_const(value);
    if (index > UINT64_MAX) {
        throw JacyException("Maximum constant pool size exceeded");
    }
    return index;
}

void Compiler::emit_const(const Value & value) {
    emit(OpCode::LOAD_CONST);
    emit(make_const(value));
}

uint64_t Compiler::id_const(Identifier * id) {
    return make_const(Value{Type::String, make_string(id->get_name())});
}

void Compiler::declare_var(Identifier * id, VarDeclKind kind) {
    if (scope_depth == 0) {
        return;
    }

    for (std::size_t i = current_scope->locals.size() - 1; i >= 0; i--) {
        const auto & local = current_scope->locals[i];
        if (local.depth != -1 && local.depth < scope_depth) {
            break;
        }

        if (id->get_name() == local.name) {
            throw JacyException(id->get_name() + " has been already declared in this scope");
        }
    }

    add_local(id, kind);
}

void Compiler::add_local(Identifier * id, VarDeclKind kind) {
    if (current_scope->locals.size() >= UINT64_MAX) {
        throw JacyException("Maximum locals stack has been exceeded");
    }

    current_scope->locals.push_back(Local{-1, id->get_name(), kind, false});
}

void Compiler::mark_inited() {
    current_scope->locals.back().depth = scope_depth;
}

void Compiler::emit_id(Identifier * id) {
    OpCode opcode;
    uint64_t resolved = resolve_local(current_scope, id->get_name());
    if (resolved != 1) {
        opcode = OpCode::GET_LOCAL;
    } else if((resolved = resolve_upvalue(current_scope, id->get_name())) != -1) {
        opcode = OpCode::GET_UPVALUE;
    } else {
        resolved = id_const(id);
        opcode = OpCode::GET_GLOBAL;
    }
    emit(opcode);
    emit(resolved);
}

void Compiler::define_var(uint64_t global) {
    if (scope_depth > 0) {
        mark_inited();
        return;
    }
    emit(OpCode::DEFINE_GLOBAL);
    emit(global);
}

uint64_t Compiler::compile_var(Identifier * id, VarDeclKind kind) {
    declare_var(id, kind);
    if (scope_depth > 0) {
        return 0;
    }
    return id_const(id);
}

uint64_t Compiler::resolve_local(const scope_ptr & scope, std::string name) {
    for (std::size_t i = scope->locals.size() - 1; i >= 0; i--) {
        if (scope->locals[i].name == name) {
            return i;
        }
    }

    return -1;
}

uint64_t Compiler::resolve_upvalue(const scope_ptr & scope, std::string name) {
    if (!scope) {
        return -1;
    }

    uint64_t local = resolve_local(scope->enclosing, name);
    if (local != -1) {
        scope->enclosing->locals[local].is_captured = true;
        return add_upvalue(scope, local, true);
    }

    uint64_t upvalue = resolve_upvalue(scope->enclosing, name);
    if (upvalue != -1) {
        return add_upvalue(scope, upvalue, false);
    }

    return -1;
}

uint64_t Compiler::add_upvalue(const scope_ptr & scope, uint64_t index, bool is_local) {
    for (std::size_t i = 0; i < scope->upvalues.size(); i++) {
        const auto & upvalue = scope->upvalues[i];
        if (upvalue.index == index && upvalue.is_local == is_local) {
            return i;
        }
    }

    scope->upvalues.push_back(Upvalue{index, is_local});
    return scope->upvalues.size() - 1;
}

void Compiler::enter_scope() {
    this->current_scope = std::make_shared<Scope>(this->current_scope);
    scope_depth++;
}

void Compiler::exit_scope() {
    scope_depth--;

    while (current_scope->locals.size() > 0 && current_scope->locals.back().depth > scope_depth) {
        if (current_scope->locals.back().is_captured) {
            // emit(OpCode::)
        }
    }

    this->current_scope = this->current_scope->enclosing;
}

void Compiler::emit(uint8_t byte) {
    current_scope->chunk.code.push_back(byte);
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

////////////////
// Statements //
////////////////
void Compiler::visit(ExprStmt * expr_stmt) {
    expr_stmt->expr->accept(*this);
    emit(OpCode::POP);
}

void Compiler::visit(Block * expr_stmt) {

}

void Compiler::visit(VarDecl * var_decl) {
    uint64_t global = compile_var(var_decl->id.get(), var_decl->kind);

    if (var_decl->assign_expr) {
        var_decl->assign_expr->accept(*this);
    }

    define_var(global);
}

void Compiler::visit(FuncDecl * func_decl) {
    uint64_t global = compile_var(func_decl->id.get(), VarDeclKind::Val);
    mark_inited();
    enter_scope();

    func_ptr func = std::make_shared<Func>();
    for (const auto & param : func_decl->params) {
        uint64_t param_const = compile_var(param.id.get(), VarDeclKind::Val);
        define_var(param_const);
    }

    func_decl->body->accept(*this);

    emit(OpCode::CLOSURE);
    emit(Value{Type::Func, func});

    exit_scope();
    define_var(global);
}

void Compiler::visit(ReturnStmt * expr_stmt) {

}

void Compiler::visit(WhileStmt * expr_stmt) {

}

void Compiler::visit(ForStmt * expr_stmt) {

}

void Compiler::visit(ClassDecl * class_decl) {
    uint64_t name_const = id_const(class_decl->id.get());
    declare_var(class_decl->id.get(), VarDeclKind::Val);

    // Define class name
    emit(OpCode::CLASS);
    emit(name_const);
    define_var(name_const);

    // TODO: Parse superclass

    emit_id(class_decl->id.get());

    for (const auto & field : class_decl->fields) {
        field->accept(*this);
    }

    emit(OpCode::POP);
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
            emit_const(Value{Type::Null});
        } break;
        case TokenType::Bool: {
            emit_const(Value{Type::Bool, literal->token.Bool() ? TrueConst : FalseConst});
        } break;
        case TokenType::Int: {
            emit_const(Value{Type::Int, make_int(static_cast<int64_t>(literal->token.Int()))});
        } break;
        case TokenType::Float: {
            emit_const(Value{Type::Float, make_float(static_cast<int64_t>(literal->token.Float()))});
        } break;
        case TokenType::String: {
            emit_const(Value{Type::String, make_string(literal->token.String())});
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

void Compiler::visit(Assign * expr_stmt) {

}

void Compiler::visit(SetExpr * expr_stmt) {

}

void Compiler::visit(GetExpr * expr_stmt) {

}

void Compiler::visit(FuncCall * expr_stmt) {
    expr_stmt->left->accept(*this);
    uint8_t args_count = expr_stmt->args.size();

    for (const auto & arg : expr_stmt->args) {
        arg->accept(*this);
    }

    emit(OpCode::CALL);
    emit(args_count);
}

void Compiler::visit(IfExpr * expr_stmt) {

}

void Compiler::visit(ListExpr * expr_stmt) {

}

void Compiler::visit(GetItem * expr_stmt) {

}

void Compiler::visit(SetItem * expr_stmt) {

}

void Compiler::visit(DictExpr * expr_stmt) {

}
