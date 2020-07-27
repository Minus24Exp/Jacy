#include "object/Bool.h"
#include "object/String.h"
#include "object/Int.h"

Bool::Bool(bool b) : Object(ObjectType::Bool, cBool), value(b)
{
    define_builtin("hash", make_nf(nullptr, "hash", {}, [this](NFArgs && args){
        return std::make_shared<Int>(static_cast<yo_int>(std::hash<bool>{}(value)));
    }));
}