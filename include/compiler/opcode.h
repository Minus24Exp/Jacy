#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <cstdint>
#include "parser/Token.h"

using Chunk = std::vector<uint8_t>;

enum class OpCode : uint8_t {
    NOP,
    CONST_NULL,
    CONST_BOOL,
    CONST_INT,
    CONST_FLOAT,
    CONST_STRING,
    LOAD,
};

const std::vector <std::string> opcodeNames {
    "NOP",
    "CONST_NULL",
    "CONST_BOOL",
    "CONST_INT",
    "CONST_FLOAT",
    "CONST_STRING",
};

struct Local {
    int depth;
    Token name;
};

#endif
