#ifndef COMPILER_H
#define COMPILER_H

#include "codegen/CodeGen.h"

class Compiler {
public:
    Compiler();
    virtual ~Compiler() = default;

    Chunk compile(const StmtList & tree);
private:
    CodeGen codegen;
};

#endif
