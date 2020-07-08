#include "object/Class.h"
#include "object/Instance.h"

obj_ptr Class::call(Interpreter & ip, ObjList && args){
	// TODO: !!! User-defined constructor

	return std::make_shared<Instance>(std::shared_ptr<Class>(this), fields);
}