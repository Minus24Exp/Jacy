#ifndef BOOL_H
#define BOOL_H

#include "Object/Object.h"

class Bool : public Object {
public:
	Bool(scope_ptr closure, const bool & b);
	virtual ~Bool() = default;

	bool get_value() const {
		return value;
	}

	bool truthy() const override {
		return value;
	}

	bool equals(Object * other) const override {
		if(other->type != ObjectType::Bool){
			return false;
		}
		return value == static_cast<Bool*>(other)->get_value();
	}

	obj_ptr clone() const override {
		return std::unique_ptr<Bool>(new Bool(closure, value));
	}

	std::string to_string() const override {
		return "<Bool:"+ std::to_string(value) + ">";
	}

private:
	bool value;
};

#endif