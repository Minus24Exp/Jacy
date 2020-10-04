#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <cstdint>
#include "vm/Value.h"
#include "parser/Token.h"
#include "tree/Stmt/VarDecl.h"

struct Chunk {
    std::vector<uint8_t> code;
    std::vector<Value> constants;
};
using OpCodeIt = std::vector<uint8_t>::iterator;

enum class OpCode : uint8_t {
    NOP,

    LOAD_CONST,

    LOAD_VAR,
    STORE_VAR,
    LOAD_UPVALUE,

    POP,

    CALL,
    MAKE_FUNC,
    CLOSURE,
};

const std::vector <std::string> opcodeNames {
//  OPCODE                  // Operands
    "NOP",                  // --

    "LOAD_NULL",            // "" -> StackTop = null
    "LOAD_BOOL",            // A (1) -> StackTop = A
    "LOAD_INT",             // A (8) -> StackTop = A
    "LOAD_FLOAT",           // A (8) -> StackTop = A
    "LOAD_STRING",          // S (8), W[S] -> StackTop = W

    "LOAD_VAR",             // A (8) -> StackTop = Variable[size - A]
    "STORE_VAR",            // A (8) -> Variable[A] = StackTop
    "LOAD_UPVALUE",         // A (8) -> Frame.closure

    "POP",                  // "" -> pop stack
    "CLOSE_UPVALUE",        // "" -> 

                            // CALL op handle 1-byte (max 256) for args count, it will be increased in the future
    "CALL",                 // A (8), C (1) -> StackTop = Variable[size - A](Stack[top...C])
    "MAKE_FUNC",            // 
    "CLOSURE",              // A (8) -> StackTop = new closure for function 
};

struct Local {
    int depth;
    std::string name;
    VarDeclKind kind;
    bool is_captured;
};

struct Upvalue {
    uint64_t index;
    bool is_local;
};

#endif
