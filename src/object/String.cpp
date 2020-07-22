#include "object/String.h"
#include "object/Int.h"

#include <sstream>

String::String(const std::string & s) : Object(ObjectType::String, cString), value(s) {}