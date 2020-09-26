#ifndef DISASM_H
#define DISASM_H

#include "compiler/opcode.h"
#include <iostream>

class Disasm {
public:
    Disasm();
    virtual ~Disasm() = default;

    void printChunk(Chunk chunk);

private:
    Chunk chunk;
    size_t index;

    uint8_t peek();
    uint8_t advance(int distance = 1);
    Chunk::iterator peek_it();
};

#endif
