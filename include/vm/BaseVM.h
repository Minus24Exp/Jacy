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
    OpCodeIt peek_it();

    // Readers //
    uint8_t read();
    uint16_t read2();
    uint32_t read4();
    uint64_t read8();

    // OpCodes //
    virtual void consumeOpCode(OpCode opcode);
    virtual void afterInstr();

    virtual void load_null() = 0;
    virtual void load_bool(bool value) = 0;
    virtual void load_int(int64_t value) = 0;
    virtual void load_float(double value) = 0;
    virtual void load_string(const char * value) = 0;
    virtual void load_var(uint64_t offset) = 0;
    virtual void store_var(uint64_t offset) = 0;
};

#endif
