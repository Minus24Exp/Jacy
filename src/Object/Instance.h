#ifndef INSTANCE_H
#define INSTANCE_H

#include "object/Class.h"

class Instance;
using instance_ptr = std::shared_ptr<Instance>;

class Instance : public Object {
public:
	Instance(Class * _class) : Object(ObjectType::Instance) {
		fields = _class->get_instance_fields();

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
		return std::make_shared<Instance>(_class.get());
	}

	std::string to_string() const {
		return "<object_"+ _class->get_name() +">";
	}

	// Instance //
	bool has(const std::string & name) const {
		return fields.find(name) != fields.end();
	}

	obj_ptr get(const std::string & name) const {
		if(!has(name)){
			return nullptr;
		}

		return fields.at(name).val;
	}

	// TODO: Rewrite for return status
	void set(const std::string & name, obj_ptr value){
		if(has(name)){
			fields.at(name).val = value;
		}
	}

private:
	LocalMap fields;
	class_ptr _class;
};

#endif