#include "object/Null.h"
#include "object/String.h"
#include "object/NativeFunc.h"
#include "object/Int.h"

Null::Null() : Object(ObjectType::Null, cNull)
{
    define_builtin("__hash", make_nf(nullptr, "__hash", {}, [](NFArgs && args){
        return make_int(static_cast<yo_int>(std::hash<Null*>{}(null_obj.get())));
    }));
}