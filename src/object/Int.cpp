#include "object/Int.h"
#include "object/String.h"
#include "object/Float.h"
#include "object/Bool.h"

Int::Int(yo_int i) : Object(ObjectType::Int, cInt), value(i)
{
    define_builtin("hash", make_nf(nullptr, "hash", {}, [this](NFArgs && args){
        return std::make_shared<Int>(static_cast<yo_int>(std::hash<yo_int>{}(value)));
    }));
}

float_ptr Int::to_float() const {
    return std::make_shared<Float>((double)value);
}