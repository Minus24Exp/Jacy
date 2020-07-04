#include "Object/String.h"
#include "Object/NativeFunc.h"

String::String(scope_ptr closure, const std::string & s)
	: Object(closure, ObjectType::String), value(s)
{
	// set("to_s", make_nf(closure, "to_s", {}, [this](NFArgs && args) -> obj_ptr {
	// 	return this->clone();
	// }));
}