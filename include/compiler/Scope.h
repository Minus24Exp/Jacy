#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <vector>

struct Scope;
using scope_ptr = std::shared_ptr<Scope>;

struct Local {
    std::string name;
    size_t depth;
};

struct Scope {
    Scope(scope_ptr parent = nullptr) : parent(parent) {}

    scope_ptr parent;

    std::vector<Local> locals;
};

#endif // SCOPE_H
