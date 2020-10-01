#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <cstring>
#include <cstdint>
#include <string>
#include <math.h>

// TODO: Optimize using QNAN mask, it will also allow writing bytecode to file

enum class Type {
    Null,
    Bool,
    Int,
    Float,
    String,
};

struct Value {
    Type tag;
    std::variant<uint8_t, long, double, const char*> val;
};

const Value NullConst = Value{Type::Null, static_cast<uint8_t>(0)};
const Value FalseConst = Value{Type::Bool, static_cast<uint8_t>(0)};
const Value TrueConst = Value{Type::Bool, static_cast<uint8_t>(1)};

#endif
