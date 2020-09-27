#include "compiler/Compiler.h"

Compiler::Compiler() {}

std::vector<uint8_t> Compiler::compile(const StmtList & tree) {
    for (const auto & stmt : tree) {
        stmt->accept(*this);
    }

    return chunk;
}

void Compiler::write(uint8_t byte) {
    chunk.push_back(byte);
}

void Compiler::write(OpCode opcode) {
    write(static_cast<uint8_t>(opcode));
}

void Compiler::write(const uint8_t * byte_array, int size) {
    for (std::size_t i = 0; i < size; i++) {
        write(byte_array[i]);
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
            write(OpCode::CONST_NULL);
        } break;
        case TokenType::Bool: {
            write(OpCode::CONST_BOOL);
            write(1);
        } break;
        case TokenType::Int: {
            write(OpCode::CONST_INT);
            yo_int val = literal->token.Int();
            write(reinterpret_cast<uint8_t*>(&val), 8);
        } break;
        case TokenType::Float: {
            write(OpCode::CONST_FLOAT);
            double val = literal->token.Float();
            write(reinterpret_cast<uint8_t*>(&val), 8);
        } break;
        case TokenType::String: {
            write(OpCode::CONST_STRING);
            std::size_t size = literal->token.String().size();
            write(reinterpret_cast<uint8_t*>(&size), sizeof(size));
            char * bytes = new char[size + 1];
            std::strcpy(bytes, literal->token.String().c_str());
            write((uint8_t*)bytes, size);
        } break;
    }
}

void Compiler::visit(Identifier * id) {
    
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
