#include "compiler/Disasm.h"

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

void Disasm::printChunk(Chunk chunk) {
    index = 0;
    this->chunk = chunk;

    while (index < chunk.size()) {
        switch (static_cast<OpCode>(peek())) {
            case OpCode::CONST_NULL: {
                std::cout << "CONST_NULL" << std::endl;
                advance();
            } break;
            case OpCode::CONST_BOOL: {
                std::cout << "CONST_BOOL ";
                advance();
                std::cout << peek() ? "(true)" : "(false)";
                advance();
            } break;
            case OpCode::CONST_INT: {
                std::cout << "CONST_INT ";
                advance();
                std::cout << bytesToLong(peek_it());
                advance(8);
            } break;
        }
    }
}