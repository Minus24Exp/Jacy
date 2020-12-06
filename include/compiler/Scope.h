#ifndef SCOPE_H
#define SCOPE_H

#include "compiler/type.h"
#include "tree/Stmt/VarDecl.h"
#include <vector>
#include <map>

namespace jc::compiler {
    struct Scope;
    using scope_ptr = std::shared_ptr<Scope>;

    struct Variable;
    using var_ptr = std::shared_ptr<Variable>;

    // Base class for Local and Compile-time global
    struct Variable {
        Variable(tree::VarDeclKind kind, type_ptr type) : kind(kind), type(type) {}

        tree::VarDeclKind kind;
        type_ptr type;
        bool is_defined{false};
    };

    struct Local : Variable {
        Local(tree::VarDeclKind kind, type_ptr type, std::string name) : Variable(kind, type), name(std::move(name)) {}

        std::string name;
        uint64_t depth{0};
        bool is_captured{false};
    };

    struct FuncLocal {
        FuncLocal(uint32_t offset, uint32_t name_offset, const func_t_ptr & signature)
            : offset(offset), name_offset(name_offset), signature(signature) {}

        uint32_t offset;
        uint32_t name_offset;
        func_t_ptr signature;
    };

    struct Scope {
        explicit Scope(scope_ptr parent = nullptr) : parent(parent) {}

        scope_ptr parent;

        std::vector<Local> locals;

        // Functions //
        std::multimap<std::string, FuncLocal> functions;
    };
}


#endif // SCOPE_H
