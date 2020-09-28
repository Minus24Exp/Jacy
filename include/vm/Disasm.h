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

    void const_null() override;
    void const_bool(bool value) override;
    void const_int(yo_int value) override;
    void const_float(double value) override;
    void const_string(const std::string & value) override;
    void load(uint64_t offset) override;
    void store(uint64_t offset) override;
};

#endif
