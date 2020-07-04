#include "Object/Object.h"
#include "Object/objects.h"

Object::Object(const ObjectType & type) : type(type), closure(closure) {
	set_field("to_s", make_nf("to_s"))
}