#ifndef OPCODE_H
#define OPCODE_H

enum class OpCode : uint8_t {
    NOP,
    CONST_NULL,
    CONST_BOOL,
    CONST_INT,
    CONST_FLOAT,
    CONST_STRING,
};

#endif
