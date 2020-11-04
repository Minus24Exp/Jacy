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

void Compiler::enter_scope() {
    scope_depth++;
}

void Compiler::exit_scope() {
    scope_depth--;
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

void Compiler::emit_int(long long int_val) {
    emit(OpCode::IntConst);
    const auto & found = int_constants.find(int_val);
    if (found != int_constants.end()) {
        emit(found->second);
        return;
    }
    chunk.constants.push_back(IntConstant{int_val});
    int_constants[int_val] = chunk.constants.size() - 1;
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
}

void Compiler::emit_string(const std::string & string_val) {
    emit(OpCode::StringConst);
    const auto & found = string_constants.find(string_val);
    if (found != string_constants.end()) {
        emit(found->second);
        return;
    }
    chunk.constants.push_back(StringConstant{string_val});
    string_constants[string_val] = chunk.constants.size() - 1;
}
