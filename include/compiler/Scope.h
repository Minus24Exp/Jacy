#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include "compiler/opcode.h"

class Scope;
using scope_ptr = std::shared_ptr<Scope>;

struct Scope {
    scope_ptr enclosing;
    std::vector<Local> locals;
    std::vector<UpValue> upvalues;
};

#endif
