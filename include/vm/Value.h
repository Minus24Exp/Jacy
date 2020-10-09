#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <cstring>
#include <cstdint>
#include <string>
#include <math.h>
#include <memory>
#include <map>

// TODO: Optimize using QNAN mask, it will also allow writing bytecode to file

class Object;
using obj_ptr = std::shared_ptr<Object>;

enum class Type {
    Null,
    Bool,
    Int,
    Float,
    String,

    Object, // User-Defined object
    Func,
    Class,
};

struct Value {
    Type type;
    obj_ptr obj;
};

struct  ObjUpvalue {
    obj_ptr obj;
};

// QNAN Version
// I need to decide how to use NaN-Boxing with separated integer and double types

// using Value = uint64_t;

// const Value SIGN_BIT = 0x8000000000000000;
// const Value QNAN = 0x7ffc000000000000;

// const Value NullConst = QNAN | 1;
// const Value FalseConst = QNAN | 2;
// const Value TrueConst = QNAN | 3;

// static inline bool is_null(Value value) {
//     return value == NullConst;
// }

// static inline bool is_bool(Value value) {
//     // value | 1, because FalseConst | 1 gives TrueConst
//     return (value | 1) == TrueConst;
// }

// static inline bool is_number(Value value) {
//     return (value & QNAN) != QNAN;
// }

// static inline bool is_obj(Value value) {
//     return ((value) & (QNAN | SIGN_BIT)) == (QNAN | SIGN_BIT);
// }

// static inline bool as_bool(Value value) {
//     return value == TrueConst;
// }

// static inline double as_num()

// static inline obj_ptr as_obj(Value value) {
//     return std::make_shared<Object>((uintptr_t)(value & !(SIGN_BIT | QNAN)));
// }

// static inline Value obj_val(const obj_ptr & obj) {
//     return static_cast<Value>(SIGN_BIT | QNAN | (uint64_t)(uintptr_t)(obj.get()));
// }


#endif
