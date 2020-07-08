#ifndef INSTANCE_H
#define INSTANCE_H

#include "object/Object.h"
#include "backend/Scope.h"

class Instance;
using instance_ptr = std::shared_ptr<Instance>;

class Instance : public Object {
public:
	Instance(const LocalMap & fields) : Object(ObjectType::Instance), fields(fields) {}
	virtual ~Instance() = default;

	bool equals(Object * other) const override {
		// TODO: Rewrite
		return true;
	}

	obj_ptr clone() const override {
		return std::make_shared<Instance>(fields);
	}

	std::string to_string() const {
		return "<Instance>";
	}

private:
	LocalMap fields;
};

#endif