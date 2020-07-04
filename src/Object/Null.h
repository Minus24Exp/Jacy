#ifndef NULL_H
#define NULL_H

#include "Object/Object.h"

class Null : public Object {
public:
	Null() : Object(ObjectType::Null) {}
	virtual ~Null() = default;

	bool truthy() const override {
		return false;
	}

	bool equals(Object * other) const override {
		return other->type == ObjectType::Null;
	}

	obj_ptr clone() const override {
		return std::unique_ptr<Null>();
	}

	std::string to_string() const override {
		return "Null";
	}
};

static inline obj_ptr make_null(){
	return std::make_unique<Null>();
}

#endif