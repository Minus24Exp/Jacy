#ifndef INT_H
#define INT_H

#include "Object/Object.h"

class Int : public Object {
public:
	Int(scope_ptr closure, const int & i);
	virtual ~Int() = default;

	int get_value() const {
		return value;
	}

	bool equals(Object * other) const override {
		if(other->type == ObjectType::Float){
			return false;
		}
		return value == static_cast<Int*>(other)->get_value();
	}

	obj_ptr clone() const override {
		return std::unique_ptr<Int>(new Int(closure, value));
	}

	std::string to_string() const override {
		return "<Int:"+ std::to_string(value) +">";
	}

private:
	int value;
};

#endif