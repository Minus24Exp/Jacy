#include "object/Bool.h"
#include "object/String.h"

Bool::Bool(bool b) : Object(ObjectType::Bool, cBool), value(b)
{
    // TODO: hash function
}