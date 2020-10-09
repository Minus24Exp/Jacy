#ifndef FUNC_H
#define FUNC_H

#include "object/Object.h"
#include "tree/Stmt/FuncDecl.h"

extern class_ptr cFunc;

class Func;
using func_ptr = std::shared_ptr<Func>;

using ParamsCountType = uint8_t;

struct Func : Object {
    Func() : Object(cFunc) {}
    ~Func() override = default;

    ParamsCountType arity() {
        return params.size();
    }

    std::string name;
    FuncParams params;
    Chunk chunk;
};

#endif
