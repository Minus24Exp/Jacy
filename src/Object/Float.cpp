#include "object/Float.h"
#include "object/String.h"
#include "object/NativeFunc.h"

Float::Float(scope_ptr closure, const double & d)
		  : Object(closure, ObjectType::Float), value(d)
{
	// set("to_s", make_nf(closure, "to_s", {}, [this](NFArgs && args) -> obj_ptr {
	// 	return std::make_unique<String>(this->closure, std::to_string(value));
	// }));
}