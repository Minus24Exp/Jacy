#ifndef CLASS_H
#define CLASS_H

#include "backend/Scope.h"

class Class;
using class_ptr = std::shared_ptr<Class>;

class Class : public Object {
public:
	Class(const std::string & name, class_ptr super, const LocalMap & fields)
		: Object(ObjectType::Class), name(name), super(super), fields(fields) {}
	virtual ~Class() = defualt;

	// Object //
	bool equals(Object * other){
		if(other->type != ObjectType::Class){
			return false;
		}
		return true;
	}

	std::string to_string() const override {
		return "<class:"+ name +">"
	}

	// Class //
	std::string get_name() const {
		return name;
	}

	LocalMap get_instance_fields() const {
		return fields;
	}

	// TODO: Add static
	// LocalMap get_static_fields() const {}

private:
	// Class cannot be copied
	obj_ptr clone() const override {
		return nullptr;
	}

	std::string name;
	class_ptr super;
	LocalMap fields;
};

#endif