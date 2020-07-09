#ifndef CLASS_H
#define CLASS_H

#include "backend/Scope.h"
#include "object/Callable.h"

class Class;
using class_ptr = std::shared_ptr<Class>;

class Class : public Object, public Callable {
public:
	Class(scope_ptr decl_scope, const std::string & name, class_ptr super)
		: Object(ObjectType::Class),
		  decl_scope(decl_scope),
		  name(name),
		  super(super),
		  fields(decl_scope->get_locals()) {}
	virtual ~Class() = default;

	// Object //
	bool equals(Object * other) const override {
		if(other->type != ObjectType::Class){
			return false;
		}
		return true;
	}

	std::string to_string() const override {
		return "<class:"+ name +">";
	}

	// Callable //
	
	// TODO: !!! Add user-defined constructors

	size_t get_required_argc() const override {
		return 0;
	}

	size_t get_max_argc() const override {
		return 0;
	}

	obj_ptr call(Interpreter & ip, ObjList && args) override;

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

	scope_ptr decl_scope;
	std::string name;
	class_ptr super;
	LocalMap fields;
};

#endif