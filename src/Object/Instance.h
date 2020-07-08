#ifndef INSTANCE_H
#define INSTANCE_H

#include "object/Object.h"
#include "backend/Scope.h"
#include "object/Class.h"

class Instance;
using instance_ptr = std::shared_ptr<Instance>;

class Instance : public Object {
public:
	Instance(class_ptr _class, const LocalMap & fields)
		: Object(ObjectType::Instance), _class(_class), fields(fields) {}
	virtual ~Instance() = default;

	bool equals(Object * other) const override {
		// TODO: Rewrite
		return true;
	}

	obj_ptr clone() const override {
		return std::make_shared<Instance>(_class, fields);
	}

	std::string to_string() const {
		return "<Instance>";
	}

	bool has(const std::string & name) const {
		if(fields.find(name) != fields.end()){
			return true;
		}
		return _class->has(name);
	}

	obj_ptr get(const std::string & name) const;

	void set(const std::string & name, obj_ptr value){
		if(fields.find(name) != fields.end()){
			fields.at(name).val = value;
		}
	}

private:
	LocalMap fields;
	class_ptr _class;
};

#endif