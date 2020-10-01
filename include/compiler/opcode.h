#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <cstdint>
#include "vm/Value.h"
#include "parser/Token.h"
#include "tree/Stmt/VarDecl.h"

using Chunk = std::vector<uint8_t>;
using OpCodeIt = Chunk::iterator;

enum class OpCode : uint8_t {
    NOP,

    LOAD_NULL,
    LOAD_BOOL,
    LOAD_INT,
    LOAD_FLOAT,
    LOAD_STRING,

    LOAD_VAR,
    STORE_VAR,
};

const std::vector <std::string> opcodeNames {
//  OPCODE                  // Operands
    "NOP",                  // --

    "LOAD_NULL",            // --
    "LOAD_BOOL",            // A (1 byte) -> R(A)
    "LOAD_INT",             // A (8 bytes) -> R(A)
    "LOAD_FLOAT",           // A (8 bytes) -> R(A)
    "LOAD_STRING",          // S (8 bytes size), Bytes[S] -> R(string from Bytes)

    "LOAD_VAR",             // 8 op bytes (stack offset)
    "STORE_VAR",            // 8 op bytes (stack offset)
};

struct Local {
    int depth;
    Token name;
    VarDeclKind kind;
};

#endif
