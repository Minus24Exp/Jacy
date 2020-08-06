#include "object/Module.h"
#include "object/String.h"

Module::Module(const LocalMap & insides, const std::string & path, const std::string & as)
    : Object(ObjectType::Module, cModule), path(path), as(as)
{
    set_fields(insides);
}

