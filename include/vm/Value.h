#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <cstring>
#include <cstdint>
#include <string>
#include <math.h>
#include <memory>

// TODO: Optimize using QNAN mask, it will also allow writing bytecode to file

class Object;
using obj_ptr = std::shared_ptr<Object>;

enum class Type {
    Null,
    Bool,
    Int,
    Float,
    String,

    // Complex types
    ObjFunc,
};

struct Value {
    Type type;
    std::variant<std::monostate, uint8_t, int64_t, double, std::string, obj_ptr> val;

    uint8_t byte() {
        return std::get<uint8_t>(val);
    }

    int64_t _long() {
        return std::get<int64_t>(val);
    }

    double _double() {
        return std::get<double>(val);
    }

    std::string string() {
        return std::get<std::string>(val);
    }

    obj_ptr obj() {
        return std::get<obj_ptr>(val);
    }
};

const Value NullConst = Value{Type::Null};
const Value FalseConst = Value{Type::Bool, static_cast<uint8_t>(0)};
const Value TrueConst = Value{Type::Bool, static_cast<uint8_t>(1)};

// For debug
const uint64_t print_offset = 0;

#endif
