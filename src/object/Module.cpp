#include "object/Module.h"

Module::Module(const std::string & as, const LocalMap & insides)
    : Object(ObjectType::Module, cModule), as(as)
{
    set_fields(insides);
}

