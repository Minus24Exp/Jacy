#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <cstdint>
#include "parser/Token.h"
#include "tree/Stmt/VarDecl.h"

using Chunk = std::vector<uint8_t>;

enum class OpCode : uint8_t {
    NOP,
    CONST_NULL,
    CONST_BOOL,
    CONST_INT,
    CONST_FLOAT,
    CONST_STRING,
    LOAD,
    STORE,
};

const std::vector <std::string> opcodeNames {
    "NOP",              // --
    "CONST_NULL",       // --
    "CONST_BOOL",       // 1 op byte
    "CONST_INT",        // 8 op bytes
    "CONST_FLOAT",      // 8 op bytes
    "CONST_STRING",     // [size] op bytes
    "LOAD",             // 8 op bytes (stack offset)
    "STORE",            // 8 op bytes (stack offset)
};

struct Local {
    int depth;
    Token name;
    VarDeclKind kind;
};

#endif
