#include "compiler/CodeGenerator.h"

CodeGenerator::CodeGenerator() {}

void CodeGenerator::write(uint8_t byte) {
    chunk.push_back(byte);
}

void CodeGenerator::write(OpCode opcode) {
    write(static_cast<uint8_t>(opcode));
}

void CodeGenerator::write(const uint8_t * byte_array, int size) {
    for (int i = 0; i < size; i++) {
        write(byte_array[i]);
    }
}

std::vector<uint8_t> CodeGenerator::gen(const StmtList & tree) {
    for (const auto & stmt : tree) {
        stmt->accept(*this);
    }

    return chunk;
}

////////////////
// Statements //
////////////////
void CodeGenerator::visit(ExprStmt * expr_stmt) {
    expr_stmt->expr->accept(*this);
}

void CodeGenerator::visit(Block * expr_stmt) {

}

void CodeGenerator::visit(VarDecl * expr_stmt) {

}

void CodeGenerator::visit(FuncDecl * expr_stmt) {

}

void CodeGenerator::visit(ReturnStmt * expr_stmt) {

}

void CodeGenerator::visit(WhileStmt * expr_stmt) {

}

void CodeGenerator::visit(ForStmt * expr_stmt) {

}

void CodeGenerator::visit(ClassDecl * expr_stmt) {

}

void CodeGenerator::visit(Import * expr_stmt) {

}

void CodeGenerator::visit(TypeDecl * expr_stmt) {

}

/////////////////
// Expressions //
/////////////////
void CodeGenerator::visit(Literal * literal) {
    // TODO: Global switch left-indent
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
            const char * bytes = literal->token.String().c_str();
            write(reinterpret_cast<const uint8_t*>(bytes), literal->token.String().size());
        } break;
    }
}

void CodeGenerator::visit(Identifier * expr_stmt) {

}

void CodeGenerator::visit(Infix * expr_stmt) {

}

void CodeGenerator::visit(Prefix * expr_stmt) {

}

void CodeGenerator::visit(Assign * expr_stmt) {

}

void CodeGenerator::visit(SetExpr * expr_stmt) {

}

void CodeGenerator::visit(GetExpr * expr_stmt) {

}

void CodeGenerator::visit(FuncCall * expr_stmt) {

}

void CodeGenerator::visit(IfExpr * expr_stmt) {

}

void CodeGenerator::visit(ListExpr * expr_stmt) {

}

void CodeGenerator::visit(GetItem * expr_stmt) {

}

void CodeGenerator::visit(SetItem * expr_stmt) {

}

void CodeGenerator::visit(DictExpr * expr_stmt) {

}
