#ifndef SCOPE_H
#define SCOPE_H

#include "compiler/type.h"
#include "tree/Stmt/VarDecl.h"
#include "common/common.h"

#include <vector>
#include <map>

namespace jc::compiler {
    struct Scope;
    struct Variable;
    using scope_ptr = std::shared_ptr<Scope>;
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
        uint32_t depth{0};
        bool is_captured{false};
    };

    struct Upvalue {
        Upvalue(uint32_t index, bool is_local) : index(index), is_local(is_local) {}

        uint32_t index;
        bool is_local;
    };

    struct Scope {
        explicit Scope(scope_ptr parent = nullptr) : parent(parent) {}

        scope_ptr parent;

        std::vector<Local> locals;
        std::vector<Upvalue> upvalues;
    };
}


#endif // SCOPE_H
