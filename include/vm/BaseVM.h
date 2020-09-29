#ifndef BASEVM_H
#define BASEVM_H

#include "compiler/opcode.h"
#include "Exception.h"
#include <stack>

class BaseVM {
public:
    BaseVM();
    virtual ~BaseVM() = default;

    virtual void eval(const Chunk & chunk);

protected:
    Chunk chunk;
    std::size_t index;
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

    virtual void load_const(uint8_t offset) = 0;
    virtual void load_var(uint64_t offset) = 0;
    virtual void store_var(uint64_t offset) = 0;
};

#endif
