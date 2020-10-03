#ifndef FUNC_H
#define FUNC_H

#include "object/Object.h"
#include "tree/Stmt/FuncDecl.h"

using ParamsCountType = uint8_t;

class Func : public Object {
public:
    Func() {}
    virtual ~Func() = default;

    // Debug call
    Value call(const std::vector<Value> & args) {
        std::string repr;
        for (std::size_t i = 0; i < args.size(); i++) {
            auto arg = args[i];
            switch (arg.type) {
                case Type::Null: {
                    repr += "null";
                } break;
                case Type::Bool: {
                    repr += arg.byte() ? "true" : "false";
                } break;
                case Type::Int: { 
                    repr += std::to_string(arg._long());
                } break;
                case Type::Float: {
                    repr += std::to_string(arg._double());
                } break;
                case Type::String: {
                    repr += arg.string();
                } break;
                default: {
                    repr += "[UNABLE TO PRINT]";
                }
                if (i < args.size() - 1) {
                    repr += ", ";
                }
            }
        }
        std::cout << repr << std::endl;
        return NullConst;
    }

    ParamsCountType arity() {
        return params.size();
    }

private:
    FuncParams params;
    std::string name;
    Chunk chunk;
};

#endif
