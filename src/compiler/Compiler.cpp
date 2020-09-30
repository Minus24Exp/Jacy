#include "compiler/Compiler.h"

Compiler::Compiler() : scope_depth(0) {}

Chunk Compiler::compile(const StmtList & tree) {
    for (const auto & stmt : tree) {
        stmt->accept(*this);
    }

    return chunk;
}

std::size_t Compiler::resolve_local(std::string name) {
    for (std::size_t i = locals.size() - 1; i >= 0; i--) {
        if (locals[i].name.String() == name) {
            return i;
        }
    }

    return -1;
}

void Compiler::addConstant(Value value) {
    chunk.constants.push_back(value);
    emit(OpCode::LOAD_CONST);
    emit(static_cast<uint64_t>(chunk.constants.size() - 1));

    // TODO: Add debug mode
    emit(OpCode::PRINT);
}

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

////////////////
// Statements //
////////////////
void Compiler::visit(ExprStmt * expr_stmt) {
    expr_stmt->expr->accept(*this);
}

void Compiler::visit(Block * expr_stmt) {

}

void Compiler::visit(VarDecl * var_decl) {
    locals.push_back({scope_depth, var_decl->id->token, var_decl->kind});

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
            addConstant(NullConst);
        } break;
        case TokenType::Bool: {
            addConstant(literal->token.Bool() ? TrueConst : FalseConst);
        } break;
        case TokenType::Int: {
            addConstant(Value{Type::Int, literal->token.Int()});
        } break;
        case TokenType::Float: {
            addConstant(Value{Type::Float, literal->token.Float()});
        } break;
        case TokenType::String: {
            addConstant(Value{Type::String, literal->token.String()});
            emit(OpCode::LOAD_CONST);
            std::size_t size = literal->token.String().size();
            emit(reinterpret_cast<uint8_t*>(&size), sizeof(size));
            char * bytes = new char[size + 1];
            std::strcpy(bytes, literal->token.String().c_str());
            emit((uint8_t*)bytes, size);
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
