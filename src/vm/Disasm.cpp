#include "vm/Disasm.h"

Disasm::Disasm() {}

void Disasm::consumeOpCode(OpCode opcode) {
    std::cout << opcodeNames[static_cast<int>(opcode)] << " ";
}

void Disasm::afterInstr() {
    std::cout << std::endl;
}

void Disasm::load_const(uint8_t offset) {
    std::cout << offset;
}

void Disasm::load_var(uint64_t offset) {
    std::cout << offset;
}

void Disasm::store_var(uint64_t offset) {
    std::cout << offset;
}