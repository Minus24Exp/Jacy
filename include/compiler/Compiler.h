#ifndef COMPILER_H
#define COMPILER_H

#include "compiler/CodeGenerator.h"

class Compiler {
public:
    Compiler();
    virtual ~Compiler() = default;

    void compile(const StmtList & tree);
private:
    CodeGenerator codegen;
};

#endif
