#ifndef SCOPE_H
#define SCOPE_H

#include "compiler/types.h"
#include "tree/Stmt/VarDecl.h"
#include <vector>
#include <map>

struct Scope;
using scope_ptr = std::shared_ptr<Scope>;

struct Local {
    Local(VarDeclKind kind, type_ptr type, std::string name) : kind(kind), type(type), name(std::move(name)) {}

    VarDeclKind kind;
    type_ptr type;
    std::string name;
    uint64_t depth;
    bool is_inited{false};
    bool is_captured{false};
};

struct Scope {
    explicit Scope(scope_ptr parent = nullptr) : parent(parent) {}

    scope_ptr parent;

    std::vector<Local> locals;
};

#endif // SCOPE_H
