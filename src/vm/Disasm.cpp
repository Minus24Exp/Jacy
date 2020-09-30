#include "vm/Disasm.h"

Disasm::Disasm() {}

void Disasm::consumeOpCode(OpCode opcode) {
    std::cout << opcodeNames[static_cast<int>(opcode)] << " ";
}

void Disasm::afterInstr() {
    std::cout << std::endl;
}

void Disasm::load_const(uint64_t offset) {
    std::cout << offset;
}

void Disasm::load_var(uint64_t offset) {
    std::cout << offset;
}

void Disasm::store_var(uint64_t offset) {
    std::cout << offset;
}


void Disasm::print() {
    Value constant = chunk.constants.back();

    switch (constant.tag) {
        case Type::Null: {
            std::cout << "null";
        } break;
        case Type::Bool: {
            std::cout << (std::get<uint8_t>(constant.val)) ? "true" : "false";
        } break;
        case Type::Int: {
            std::cout << std::get<long>(constant.val);
        } break;
        case Type::Float: {
            std::cout << std::get<double>(constant.val);
        } break;
        default: {
            std::cout << "Unsupported type";
        }
    }
}
