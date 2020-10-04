#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include "compiler/opcode.h"

class Scope;
using scope_ptr = std::shared_ptr<Scope>;

struct Scope {
    scope_ptr enclosing;
    Chunk chunk;
    std::vector<Local> locals;
    std::vector<Upvalue> upvalues;

    Scope(const scope_ptr & enclosing) : enclosing(enclosing) {}
    virtual ~Scope() = default;
};

#endif
