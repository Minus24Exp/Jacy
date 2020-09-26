#include "codegen/CodeGen.h"

CodeGen::CodeGen() {}

std::vector<uint8_t> CodeGen::gen(const StmtList & tree) {
    for (const auto & stmt : tree) {
        stmt->accept(*this);
    }

    return chunk;
}

void CodeGen::write(uint8_t byte) {
    chunk.push_back(byte);
}

void CodeGen::write(OpCode opcode) {
    write(static_cast<uint8_t>(opcode));
}

void CodeGen::write(const uint8_t * byte_array, int size) {
    for (int i = 0; i < size; i++) {
        write(byte_array[i]);
    }
}

////////////////
// Statements //
////////////////
void CodeGen::visit(ExprStmt * expr_stmt) {
    expr_stmt->expr->accept(*this);
}

void CodeGen::visit(Block * expr_stmt) {

}

void CodeGen::visit(VarDecl * expr_stmt) {

}

void CodeGen::visit(FuncDecl * expr_stmt) {

}

void CodeGen::visit(ReturnStmt * expr_stmt) {

}

void CodeGen::visit(WhileStmt * expr_stmt) {

}

void CodeGen::visit(ForStmt * expr_stmt) {

}

void CodeGen::visit(ClassDecl * expr_stmt) {

}

void CodeGen::visit(Import * expr_stmt) {

}

void CodeGen::visit(TypeDecl * expr_stmt) {

}

/////////////////
// Expressions //
/////////////////
void CodeGen::visit(Literal * literal) {
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
            const char * bytes = literal->token.String().c_str();
            write(reinterpret_cast<uint8_t*>(&size), 8);
            write(reinterpret_cast<const uint8_t*>(bytes), size);
        } break;
    }
}

void CodeGen::visit(Identifier * expr_stmt) {

}

void CodeGen::visit(Infix * expr_stmt) {

}

void CodeGen::visit(Prefix * expr_stmt) {

}

void CodeGen::visit(Assign * expr_stmt) {

}

void CodeGen::visit(SetExpr * expr_stmt) {

}

void CodeGen::visit(GetExpr * expr_stmt) {

}

void CodeGen::visit(FuncCall * expr_stmt) {

}

void CodeGen::visit(IfExpr * expr_stmt) {

}

void CodeGen::visit(ListExpr * expr_stmt) {

}

void CodeGen::visit(GetItem * expr_stmt) {

}

void CodeGen::visit(SetItem * expr_stmt) {

}

void CodeGen::visit(DictExpr * expr_stmt) {

}
