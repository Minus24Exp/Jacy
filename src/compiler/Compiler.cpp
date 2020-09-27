#include "compiler/Compiler.h"

Compiler::Compiler() : scope_depth(0) {}

std::vector<uint8_t> Compiler::compile(const StmtList & tree) {
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

////////////////
// Statements //
////////////////
void Compiler::visit(ExprStmt * expr_stmt) {
    expr_stmt->expr->accept(*this);
}

void Compiler::visit(Block * expr_stmt) {

}

void Compiler::visit(VarDecl * expr_stmt) {

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
            emit(OpCode::CONST_NULL);
        } break;
        case TokenType::Bool: {
            emit(OpCode::CONST_BOOL);
            emit(1);
        } break;
        case TokenType::Int: {
            emit(OpCode::CONST_INT);
            yo_int val = literal->token.Int();
            emit(reinterpret_cast<uint8_t*>(&val), 8);
        } break;
        case TokenType::Float: {
            emit(OpCode::CONST_FLOAT);
            double val = literal->token.Float();
            emit(reinterpret_cast<uint8_t*>(&val), 8);
        } break;
        case TokenType::String: {
            emit(OpCode::CONST_STRING);
            std::size_t size = literal->token.String().size();
            emit(reinterpret_cast<uint8_t*>(&size), sizeof(size));
            char * bytes = new char[size + 1];
            std::strcpy(bytes, literal->token.String().c_str());
            emit((uint8_t*)bytes, size);
        } break;
    }
}

void Compiler::visit(Identifier * id) {
    emit(OpCode::LOAD);
    emit(resolve_local(id->get_name()));
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
