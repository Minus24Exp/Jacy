#include "object/Object.h"
#include "object/String.h"
#include "object/NativeFunc.h"

Object::Object(scope_ptr closure, const ObjectType & type)
	: closure(closure), type(type)
{
	// set("to_s", make_nf(closure, "to_s", {}, [this](NFArgs && args) -> obj_ptr {
	// 	return std::make_unique<String>(this->closure, "<object>");
	// }));
}