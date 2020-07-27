#include "object/String.h"
#include "object/Int.h"

#include <sstream>

String::String(const std::string & s) : Object(ObjectType::String, cString), value(s)
{
    define_builtin("hash", make_nf(nullptr, "hash", {}, [this](NFArgs && args){
        return std::make_shared<Int>(static_cast<yo_int>(std::hash<std::string>{}(value)));
    }));
}