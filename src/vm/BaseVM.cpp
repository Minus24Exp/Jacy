#include "vm/BaseVM.h"

BaseVM::BaseVM() {}

uint8_t BaseVM::peek() {
    return chunk.code[index];
}

uint8_t BaseVM::advance(int distance) {
    index += distance;
    return chunk.code[index];
}

OpCodeIt BaseVM::peek_it() {
    return chunk.code.begin() + index;
}

uint8_t BaseVM::read_byte() {
    uint8_t b = peek();
    advance();
    return b;
}

uint16_t BaseVM::read_short() {
    uint16_t s;
    std::copy(peek_it(), peek_it() + 2, reinterpret_cast<uint8_t*>(&s));
    advance(2);
    return s;
}

uint32_t BaseVM::read_int() {
    uint32_t i;
    std::copy(peek_it(), peek_it() + 4, reinterpret_cast<uint8_t*>(&i));
    advance(4);
    return i;
}

uint64_t BaseVM::read_long() {
    uint64_t l;
    std::copy(peek_it(), peek_it() + 8, reinterpret_cast<uint8_t*>(&l));
    advance(8);
    return l;
}

void BaseVM::consumeOpCode(OpCode opcode) {}
void BaseVM::afterInstr() {}

void BaseVM::eval(const Chunk & chunk) {
    index = 0;
    this->chunk = chunk;

    while (index < chunk.code.size()) {
        OpCode opcode = static_cast<OpCode>(peek());
        advance();
        consumeOpCode(opcode);

        switch (opcode) {
            case OpCode::NOP: {}
            case OpCode::LOAD_CONST: {
                uint8_t offset = read_long();
                load_const(offset);
            } break;
            case OpCode::LOAD_VAR: {
                std::size_t offset = read_long();
                load_var(offset);
            } break;
            case OpCode::STORE_VAR: {
                std::size_t offset = read_long();
                store_var(offset);
            } break;
            case OpCode::PRINT: {
                print();
            } break;
            default: {
                throw DevError("[Disasm] Unknown opcode " + std::to_string(peek()));
            }
        }

        afterInstr();
    }
}
