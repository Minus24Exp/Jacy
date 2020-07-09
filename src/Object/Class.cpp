#include "object/Class.h"
#include "object/Instance.h"

obj_ptr Class::call(Interpreter & ip, ObjList && args){
	// TODO: Add user-defined constructors

	return std::make_shared<Instance>(this);
}