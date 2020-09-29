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

    void load_const(uint64_t offset) override;
    void load_var(uint64_t offset) override;
    void store_var(uint64_t offset) override;

    void print() override;
};

#endif
