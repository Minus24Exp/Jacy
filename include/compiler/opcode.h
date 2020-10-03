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

    POP,

    CALL,

    MAKE_FUNC,
};

const std::vector <std::string> opcodeNames {
//  OPCODE                  // Operands
    "NOP",                  // --

    "LOAD_NULL",            // --
    "LOAD_BOOL",            // A (1) -> StackTop = A
    "LOAD_INT",             // A (8) -> StackTop = A
    "LOAD_FLOAT",           // A (8) -> StackTop = A
    "LOAD_STRING",          // S (8), Bytes[S - size] -> StackTop = String from Bytes

    "LOAD_VAR",             // A (8) -> StackTop = Variable[size - A]
    "STORE_VAR",            // A (8) -> Variable[A] = StackTop

    "POP",                  // 

                            // CALL op handle 1-byte (max 256) for args count, it will be increased in the future
    "CALL",                 // A (8 bytes offset), C (1) -> StackTop = Variable[size - A](Stack[top...C])

    "MAKE_FUNC",            // 
};

struct Local {
    int depth;
    std::string name;
    VarDeclKind kind;
};

#endif
