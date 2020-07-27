#include "object/Null.h"
#include "object/String.h"
#include "object/NativeFunc.h"
#include "object/Int.h"

Null::Null() : Object(ObjectType::Null, cNull)
{
    define_builtin("hash", make_nf(nullptr, "hash", {}, [](NFArgs && args){
        return std::make_shared<Int>(0);
    }));
}