#include "compiler/Compiler.h"

Compiler::Compiler() : scope_depth(0) {
    // Debug print function
    // locals.emplace(locals.begin() + print_offset, Local{0, "print", VarDeclKind::Val});
}

Chunk Compiler::compile(const StmtList & tree) {
    for (const auto & stmt : tree) {
        stmt->accept(*this);
    }

    return chunk;
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

// void Compiler::addConstant(Value value) {
//     chunk.constants.push_back(value);
//     emit(static_cast<uint64_t>(chunk.constants.size() - 1));

//     // TODO: Add debug mode
//     emit(OpCode::PRINT);
// }

void Compiler::emit(uint8_t byte) {
    chunk.push_back(byte);
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
    current_scope->locals.push_back({scope_depth, var_decl->id->get_name(), var_decl->kind});

    if (var_decl->assign_expr) {
        var_decl->assign_expr->accept(*this);
        emit(OpCode::STORE_VAR);
        emit(current_scope->locals.size() - 1);
    }
}

void Compiler::visit(FuncDecl * expr_stmt) {
    
}

void Compiler::visit(ReturnStmt * expr_stmt) {

}

void Compiler::visit(WhileStmt * expr_stmt) {

}

void Compiler::visit(ForStmt * expr_stmt) {

}

void Compiler::visit(ClassDecl * expr_stmt) {

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
            emit(OpCode::LOAD_NULL);
        } break;
        case TokenType::Bool: {
            emit(OpCode::LOAD_BOOL);
            emit(static_cast<uint8_t>(literal->token.Bool()));
        } break;
        case TokenType::Int: {
            emit(OpCode::LOAD_INT);
            emit(static_cast<uint64_t>(literal->token.Int()));
        } break;
        case TokenType::Float: {
            emit(OpCode::LOAD_FLOAT);
            emit(static_cast<uint64_t>(literal->token.Float()));
        } break;
        case TokenType::String: {
            emit(OpCode::LOAD_STRING);
            const std::string & s = literal->token.String();
            emit(static_cast<uint64_t>(s.size()));
            emit(reinterpret_cast<const uint8_t*>(s.c_str()), s.size());
        } break;
    }
}

void Compiler::visit(Identifier * id) {
    uint64_t resolved = resolve_local(current_scope, id->get_name());
    if (resolved != 1) {
        emit(OpCode::LOAD_VAR);
    } else if((resolved = resolve_upvalue(current_scope, id->get_name())) != -1) {
        emit(OpCode::LOAD_UPVALUE);
    } else {
        throw new JacyException(id->get_name() + " is not defined");
    }
    emit(resolved);
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
