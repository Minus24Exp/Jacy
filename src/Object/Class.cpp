#include "Object/Class.h"

Class::Class(const std::string & name, Class * super, const ClassFields & fields){
	this->name = name;
	this->super = super;
	this->fields = fields;
}

Object * Class::find_field(const std::string & name) const {
	if(fields.find(name) != fields.end()){
		return fields.at(name);
	}

	if(super){
		return super.find_field(name);
	}

	return nullptr;
}