#include "codegen/Disasm.h"

Disasm::Disasm() {}

uint8_t Disasm::peek() {
    return chunk[index];
}

uint8_t Disasm::advance(int distance) {
    index += distance;
    return chunk[index];
}

Chunk::iterator Disasm::peek_it() {
    return chunk.begin() + index;
}

void Disasm::consumeOpcode() {
    std::cout << opcodeNames[peek()] << " ";
    advance();
}

void Disasm::printChunk(Chunk chunk) {
    index = 0;
    this->chunk = chunk;

    while (index < chunk.size()) {
        switch (static_cast<OpCode>(peek())) {
            case OpCode::CONST_NULL: {
                consumeOpcode();
            } break;
            case OpCode::CONST_BOOL: {
                consumeOpcode();
                std::cout << peek() ? "(true)" : "(false)";
                advance();
            } break;
            case OpCode::CONST_INT: {
                consumeOpcode();
                std::cout << bytesToLong(peek_it());
                advance(8);
            } break;
            case OpCode::CONST_FLOAT: {
                consumeOpcode();
                std::cout << bytesToDouble(peek_it());
                advance(8);
            } break;
            case OpCode::CONST_STRING: {
                consumeOpcode();
                std::size_t size = bytesToLong(peek_it());
                advance(8);
                std::cout << bytesToString(peek_it(), size);
                advance(size);
            } break;
        }
        std::cout << std::endl;
    }
}