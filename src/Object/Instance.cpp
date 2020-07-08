#include "object/Instance.h"
#include "object/Callable.h"

obj_ptr Instance::get(const std::string & name) const {
	// Get instance field
	if(fields.find(name) != fields.end()){
		return fields.at(name).val;
	}
	
	// Get class field, otherwise
	
	obj_ptr class_field = _class->get(name);

	if(!class_field){
		return nullptr;
	}

	if(class_field->type == ObjectType::Callable){
		callable_ptr func = std::static_pointer_cast<Callable>(class_field);
		return func->bind(clone());
	}

	return class_field;
}