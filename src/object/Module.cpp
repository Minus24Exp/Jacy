#include "object/Module.h"
#include "object/String.h"

Module::Module(const LocalMap & insides)
    : Object(ObjectType::Module, cModule)
{
    set_fields(insides);
}

