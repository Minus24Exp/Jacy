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

uint8_t BaseVM::read() {
    uint8_t b = peek();
    advance();
    return b;
}

uint16_t BaseVM::read2() {
    uint16_t s;
    std::copy(peek_it(), peek_it() + 2, reinterpret_cast<uint8_t*>(&s));
    advance(2);
    return s;
}

uint32_t BaseVM::read4() {
    uint32_t i;
    std::copy(peek_it(), peek_it() + 4, reinterpret_cast<uint8_t*>(&i));
    advance(4);
    return i;
}

uint64_t BaseVM::read8() {
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
        OpCode opcode = static_cast<OpCode>(read());
        consumeOpCode(opcode);

        switch (opcode) {
            case OpCode::NOP: {}
            case OpCode::LOAD_NULL: {
                load_null();
            } break;
            case OpCode::LOAD_BOOL: {
                uint8_t value = read();
                load_bool(static_cast<bool>(value));
            } break;
            case OpCode::LOAD_INT: {
                uint64_t value = read8();
                load_int(static_cast<int64_t>(value));
            } break;
            case OpCode::LOAD_FLOAT: {
                uint64_t value = read8();
                load_int(static_cast<double>(value));
            } break;
            case OpCode::LOAD_STRING: {
                uint64_t size = read8();
                load_string(std::string(peek_it(), peek_it() + size).c_str());
                advance(size);
            } break;
            case OpCode::LOAD_VAR: {
                uint64_t offset = read8();
                load_var(offset);
            } break;
            case OpCode::STORE_VAR: {
                std::size_t offset = read8();
                store_var(offset);
            } break;
            case OpCode::POP: {
                pop();
            } break;
            case OpCode::CALL: {
                uint8_t args_count = read();
                call(args_count);
            } break;
            default: {
                throw DevError("[Disasm] Unknown opcode " + std::to_string(peek()));
            }
        }

        afterInstr();
    }
}
