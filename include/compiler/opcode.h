#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <cstdint>
#include "parser/Token.h"
#include "tree/Stmt/VarDecl.h"

using Chunk = std::vector<uint8_t>;

enum class OpCode : uint8_t {
    NOP,
    LOAD_CONST,
    LOAD_VAR,
    STORE_VAR,
};

const std::vector <std::string> opcodeNames {
//  OPCODE                  // Operands
    "NOP",                  // --
    "LOAD_CONST",           // 1 op byte
    "LOAD_VAR",             // 8 op bytes (stack offset)
    "STORE_VAR",            // 8 op bytes (stack offset)
};

struct Local {
    int depth;
    Token name;
    VarDeclKind kind;
};

#endif
