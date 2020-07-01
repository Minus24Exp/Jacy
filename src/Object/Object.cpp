#include "Object/Object.h"
#include "Object/NativeMethod.h"

Object::Object(){
	super = nullptr;
}

void Object::extend(Object * super){
	this->super = super;
}

bool Object::has_field(const std::string & name) const {
	return fields.find(name) != fields.end();
}

Object * Object::find_field(const std::string & name) const {
	if(has_field(name)){
		return fields.at(name);
	}

	if(super){
		return super->find_field(name);
	}

	return nullptr;
}

void Object::set_field(const std::string & name, Object * value){
	fields[name] = value;
}

// Helpers
bool Object::has_method(const std::string & name) const {
	return has_field(name) && dynamic_cast<Callable*>(find_field(name));
}

Callable * Object::find_method(const std::string & name) const {
	return dynamic_cast<Callable*>(find_field(name));
}

void Object::set_return_method(const std::string & name, Object * val){
	set_field(name, make_native_method([&](const NativeMethodArgs & args){
		return val;
	}));
}

