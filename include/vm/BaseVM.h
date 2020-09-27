#ifndef BASEVM_H
#define BASEVM_H

#include "codegen/opcode.h"
#include "Exception.h"

class BaseVM {
public:
    BaseVM();
    virtual ~BaseVM() = default;

    virtual void eval(const Chunk & chunk);

protected:
    Chunk chunk;
    size_t index;
    uint8_t peek();
    uint8_t advance(int distance = 1);
    Chunk::iterator peek_it();

    // Readers //
    uint8_t read_byte();
    uint16_t read_short();
    uint32_t read_int();
    uint64_t read_long();

    // OpCodes //
    virtual void consumeOpCode(OpCode opcode);
    virtual void afterInstr();
    
    virtual void const_null() = 0;
    virtual void const_bool(bool value) = 0;
    virtual void const_int(yo_int value) = 0;
    virtual void const_float(double value) = 0;
    virtual void const_string(std::string value) = 0;
};

#endif
