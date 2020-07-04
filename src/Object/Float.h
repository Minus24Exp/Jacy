#ifndef FLOAT_H
#define FLOAT_H

#include "object/Object.h"

class Float : public Object {
public:
	Float(scope_ptr closure, const double & d);
	virtual ~Float() = default;

	double get_value() const {
		return value;
	}

	bool equals(Object * other) const override {
		if(other->type != ObjectType::Float){
			return false;
		}
		return value == static_cast<Float*>(other)->get_value();
	}

	obj_ptr clone() const override {
		return std::unique_ptr<Float>(new Float(closure, value));
	}

	std::string to_string() const override {
		return "<Float:"+ std::to_string(value) + ">";
	}

private:
	double value;
};

#endif