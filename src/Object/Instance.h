#ifndef INSTANCE_H
#define INSTANCE_H

#include "object/Class.h"
#include "backend/Scope.h"

class Instance;
using instance_ptr = std::shared_ptr<Instance>;

class Instance : public Object, public Scope {
public:
	Instance(scope_ptr closure, Class * _class)
			: Object(ObjectType::Instance),
		  	  closure(closure),
		  	  Scope(closure, _class->get_instance_fields())
	{
		this->_class = std::shared_ptr<Class>(_class);
	}
	virtual ~Instance() = default;

	// Object //
	bool equals(Object * other) const override {
		if(other->type != ObjectType::Instance){
			return false;
		}
		return true;
	}

	obj_ptr clone() const override {
		return std::make_shared<Instance>(closure, _class.get());
	}

	std::string to_string() const {
		return "<object_"+ _class->get_name() +">";
	}

private:
	scope_ptr closure;
	class_ptr _class;
};

#endif