#ifndef SCOPE_H
#define SCOPE_H

#include "compiler/types.h"
#include "tree/Stmt/VarDecl.h"
#include <vector>
#include <map>

struct Scope;
using scope_ptr = std::shared_ptr<Scope>;

struct Variable;
using var_ptr = std::shared_ptr<Variable>;

// Base class for Local and Compile-time global
struct Variable {
    Variable(VarDeclKind kind, type_ptr type) : kind(kind), type(type) {}

    VarDeclKind kind;
    type_ptr type;
    bool is_defined{false};
};

struct Local : Variable {
    Local(VarDeclKind kind, type_ptr type, std::string name) : Variable(kind, type), name(std::move(name)) {}

    std::string name;
    uint64_t depth{0};
    bool is_captured{false};
};

struct Scope {
    explicit Scope(scope_ptr parent = nullptr) : parent(parent) {}

    scope_ptr parent;

    std::vector<Local> locals;
};

#endif // SCOPE_H
