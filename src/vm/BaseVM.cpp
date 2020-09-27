#include "vm/BaseVM.h"

BaseVM::BaseVM() {}

uint8_t BaseVM::peek() {
    return chunk[index];
}

uint8_t BaseVM::advance(int distance) {
    index += distance;
    return chunk[index];
}

Chunk::iterator BaseVM::peek_it() {
    return chunk.begin() + index;
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

    while (index < chunk.size()) {
        OpCode opcode = static_cast<OpCode>(peek());
        advance();
        consumeOpCode(opcode);

        switch (opcode) {
            case OpCode::CONST_NULL: {
                const_null();
            } break;
            case OpCode::CONST_BOOL: {
                const_bool(static_cast<bool>(read_byte()));
            } break;
            case OpCode::CONST_INT: {
                const_int(static_cast<yo_int>(read_long()));
            } break;
            case OpCode::CONST_FLOAT: {
                const_float(static_cast<double>(read_long()));
            } break;
            case OpCode::CONST_STRING: {
                std::size_t size;
                size = sizeof(size) == 8 ? read_long() : read_int();
                const_string(std::string(peek_it(), peek_it() + size));
                advance(size);
            } break;
            default: {
                throw DevError("[Disasm] Unknown opcode " + std::to_string(peek()));
            }
        }

        afterInstr();
    }
}
