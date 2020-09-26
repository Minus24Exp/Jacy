#include "codegen/Compiler.h"

Compiler::Compiler() {}

Chunk Compiler::compile(const StmtList & tree) {
    return codegen.gen(tree);
}