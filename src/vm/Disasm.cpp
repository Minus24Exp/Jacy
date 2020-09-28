#include "vm/Disasm.h"

Disasm::Disasm() {}

void Disasm::consumeOpCode(OpCode opcode) {
    std::cout << opcodeNames[static_cast<int>(opcode)] << " ";
}

void Disasm::afterInstr() {
    std::cout << std::endl;
}

void Disasm::const_null() {}

void Disasm::const_bool(bool value) {
    std::cout << (value ? "(true)" : "(false)");
}

void Disasm::const_int(yo_int value) {
    std::cout << value;
}

void Disasm::const_float(double value) {
    std::cout << std::setprecision(64) << value;
}

void Disasm::const_string(const std::string & value) {
    std::cout << value;
}

void Disasm::load(uint64_t offset) {
    std::cout << offset;
}

void Disasm::store(uint64_t offset) {
    std::cout << offset;
}