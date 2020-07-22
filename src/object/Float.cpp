#include "object/Float.h"
#include "object/String.h"

Float::Float(double d) : Object(ObjectType::Float, cFloat), value(d) {}