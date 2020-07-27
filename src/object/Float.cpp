#include "object/Float.h"
#include "object/String.h"
#include "object/Int.h"

Float::Float(double d) : Object(ObjectType::Float, cFloat), value(d)
{   
    define_builtin("hash", make_nf(nullptr, "hash", {}, [this](NFArgs && args){
        return std::make_shared<Int>(static_cast<yo_int>(std::hash<double>{}(value)));
    }));
}