#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <utility>
#include "compiler/opcode.h"

class Scope;
using scope_ptr = std::shared_ptr<Scope>;

struct Scope {
    scope_ptr enclosing;
    Chunk chunk;
    std::vector<Local> locals;
    std::vector<Upvalue> upvalues;

    explicit Scope(scope_ptr enclosing) : enclosing(std::move(enclosing)) {}
    virtual ~Scope() = default;
};

#endif
