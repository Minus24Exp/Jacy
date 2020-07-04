#include "object/Null.h"
#include "object/String.h"
#include "object/NativeFunc.h"

Null::Null(scope_ptr closure) : Object(closure, ObjectType::Null) {
	// set("to_s", make_nf(closure, "to_s", {}, [this](NFArgs && args) -> obj_ptr {
	// 	return std::make_unique<String>(this->closure, "Null");
	// }));
}