#ifndef COMPILER_H
#define COMPILER_H

#include "tree/Stmt/Stmt.h"

class Compiler {
public:
    Compiler();
    virtual ~Compiler() = default;

    void compile(const StmtList & tree);
private:
    StmtList tree; 
};

#endif