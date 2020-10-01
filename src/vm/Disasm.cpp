#include "vm/Disasm.h"

Disasm::Disasm() {}

void Disasm::consumeOpCode(OpCode opcode) {
    std::cout << opcodeNames[static_cast<int>(opcode)] << " ";
}

void Disasm::afterInstr() {
    std::cout << std::endl;
}

void Disasm::load_null() {}

void Disasm::load_bool(bool value) {
    std::cout << value ? "true" : "false";
}

void Disasm::load_int(int64_t value) {
    std::cout << value;
}

void Disasm::load_float(double value) {
    std::cout << value;
}

void Disasm::load_string(const char * value) {
    std::cout << value;
}

void Disasm::load_var(uint64_t offset) {
    std::cout << offset;
}

void Disasm::store_var(uint64_t offset) {
    std::cout << offset;
}
