#include "object/Int.h"
#include "object/String.h"
#include "object/NativeFunc.h"

Int::Int(scope_ptr closure, const int & i)
		  : Object(closure, ObjectType::Int), value(i)
{
	// set("to_s", make_nf(closure, "to_s", {}, [this](NFArgs && args) -> obj_ptr {
	// 	return std::make_unique<String>(this->closure, std::to_string(value));
	// }));
}