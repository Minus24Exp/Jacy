#include "Object/Class.h"

Class::Class(const ObjectFields & fields) : fields(fields) {
	set_field("new", make_native_method([this](const NMArgs & args){
		// TODO: Add constructor method
		Object * instance = new Object;

		// TODO: Add super field inheritance

		for(const auto & field : fields){
			instance->set_field(field.first, field.second);
		}

		return instance;
	}));
}

void Class::extend(Class * super){
	this->super = super;
}
