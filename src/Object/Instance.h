#ifndef INSTANCE_H
#define INSTANCE_H

class Instance : public Object {
public:
	Instance(class_ptr _class) : Object(ObjectType::Instance), _class(_class) {
		fields = _class->get_instance_fields();
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
		return std::make_shared<Instance>(_class);
	}

	std::string to_string() const {
		return "<object_"+ _class->get_name() +">"
	}

	// Instance //
	bool has(const std::string & name) const {
		return fields.find(name) != fields.end();
	}

	obj_ptr get(const std::string & name) const {
		const auto it = fields.find(name);

		if(it == fields.end()){
			return nullptr;
		}

		if(it->second.val->type == ObjectType::Callable){
			
		}

		if(has(name)){
			return fields.at(name).val;
		}
		return nullptr;
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