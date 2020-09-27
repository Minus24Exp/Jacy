#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <cstdint>
#include "parser/Token.h"

enum class OpCode : uint8_t {
    NOP,
    CONST_NULL,
    CONST_BOOL,
    CONST_INT,
    CONST_FLOAT,
    CONST_STRING,
};

const std::vector <std::string> opcodeNames {
    "NOP",
    "CONST_NULL",
    "CONST_BOOL",
    "CONST_INT",
    "CONST_FLOAT",
    "CONST_STRING",
};

using Chunk = std::vector<uint8_t>;

static inline yo_int bytesToLong(Chunk::iterator it) {
    yo_int value;
    std::copy(it, it + 8, reinterpret_cast<uint8_t*>(&value));
    return value;
}

static inline double bytesToDouble(Chunk::iterator it) {
    double value;
    std::copy(it, it + 8, reinterpret_cast<uint8_t*>(&value));
    return value;
}

// static inline std::string bytesToString(Chunk::iterator it, std::size_t size) {
//     return std::string(it, it + size);
// }

#endif
