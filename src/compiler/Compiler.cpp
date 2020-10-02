#include "compiler/Compiler.h"

Compiler::Compiler() : scope_depth(0) {
    // Debug print function
    locals.emplace(locals.begin() + print_offset, Local{0, "print", VarDeclKind::Val});
}

Chunk Compiler::compile(const StmtList & tree) {
    for (const auto & stmt : tree) {
        stmt->accept(*this);
    }

    return chunk;
}

std::size_t Compiler::resolve_local(std::string name) {
    for (std::size_t i = locals.size() - 1; i >= 0; i--) {
        if (locals[i].name == name) {
            return i;
        }
    }

    return -1;
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
    locals.push_back({scope_depth, var_decl->id->get_name(), var_decl->kind});

    if (var_decl->assign_expr) {
        var_decl->assign_expr->accept(*this);
        emit(OpCode::STORE_VAR);
        emit(locals.size() - 1);
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
    emit(OpCode::LOAD_VAR);
    std::size_t resolved = resolve_local(id->get_name());
    if (resolved == -1) {
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
