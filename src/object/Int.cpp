#include "object/Int.h"
#include "object/String.h"
#include "object/Float.h"
#include "object/Bool.h"

Int::Int(yo_int i) : Object(ObjectType::Int, cInt), value(i) {}

float_ptr Int::to_float() const {
    return std::make_shared<Float>((double)value);
}