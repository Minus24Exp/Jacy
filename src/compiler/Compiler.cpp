#include "compiler/Compiler.h"

Compiler::Compiler() {}

void Compiler::compile(const StmtList & tree) {
    Chunk chunk = codegen.gen(tree);

    for (const auto & instr : chunk) {
        std::cout << (int)instr << " ";
    }
}