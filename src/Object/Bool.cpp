#include "Object/Bool.h"
#include "Object/String.h"
#include "Object/NativeFunc.h"

Bool::Bool(scope_ptr closure, const bool & b)
		  : Object(closure, ObjectType::Bool), value(b)
{
	// set("to_s", make_nf(closure, "to_s", {}, [this](NFArgs && args) -> obj_ptr {
	// 	return std::make_unique<String>(this->closure, std::to_string(value));
	// }));
}