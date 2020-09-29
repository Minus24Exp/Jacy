#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <cstdint>
#include "vm/Value.h"
#include "parser/Token.h"
#include "tree/Stmt/VarDecl.h"

using OpCodeIt = std::vector<uint64_t>::iterator;

struct Chunk {
    std::vector<uint64_t> code;
    std::vector<Value> constants;
};

enum class OpCode : uint8_t {
    NOP,
    LOAD_CONST,
    LOAD_VAR,
    STORE_VAR,

    PRINT, // Debug only -- remove in prod
};

const std::vector <std::string> opcodeNames {
//  OPCODE                  // Operands
    "NOP",                  // --
    "LOAD_CONST",           // 1 op byte
    "LOAD_VAR",             // 8 op bytes (stack offset)
    "STORE_VAR",            // 8 op bytes (stack offset)

    "PRINT",
};

struct Local {
    int depth;
    Token name;
    VarDeclKind kind;
};

#endif
