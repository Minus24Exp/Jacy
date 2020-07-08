#include "object/Class.h"
#include "object/Instance.h"

obj_ptr Class::call(Interpreter & ip, ObjList && args){
	// TODO: !!! User-defined constructor
	// TODO: !!! Super class fields inheritance

	return std::make_shared<Instance>(closure->get_locals());
}