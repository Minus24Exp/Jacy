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
        std::map<std::string, FuncLocal> functions;
    };
}


#endif // SCOPE_H
