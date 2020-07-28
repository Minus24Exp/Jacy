#include "object/String.h"
#include "object/Int.h"

#include <sstream>

String::String(const std::string & s) : Object(ObjectType::String, cString), value(s)
{
    define_builtin("hash", make_nf(nullptr, "hash", {}, [this](NFArgs && args){
        return make_int(static_cast<yo_int>(std::hash<std::string>{}(value)));
    }));

    define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return shared_from_this();
    }));
}