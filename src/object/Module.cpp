#include "object/Module.h"
#include "object/String.h"

Module::Module(const std::string & as, const LocalMap & insides)
    : Object(ObjectType::Module, cModule), as(as)
{
    set_fields(insides);
}

