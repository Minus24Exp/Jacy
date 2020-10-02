#ifndef DISASM_H
#define DISASM_H

#include <iostream>
#include <iomanip>
#include "vm/BaseVM.h"

class Disasm : public BaseVM {
public:
    Disasm();
    virtual ~Disasm() = default;

private:
    void consumeOpCode(OpCode opcode) override;
    void afterInstr() override;

    void load_null() override;
    void load_bool(bool value) override;
    void load_int(int64_t value) override;
    void load_float(double value) override;
    void load_string(const char * value) override;
    void load_var(uint64_t offset) override;
    void store_var(uint64_t offset) override;
    void pop() override;
    void call(uint8_t args_count) override;
};

#endif
