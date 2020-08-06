#include "object/Module.h"
#include "object/String.h"

Module::Module(const LocalMap & insides, const std::string & path)
    : Object(ObjectType::Module, cModule), path(path)
{
    set_fields(insides);
}

