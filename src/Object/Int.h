#ifndef INT_H
#define INT_H

class Int : public Object {
public:
	Int(const int & i) : Object(ObjectType::Int), value(i) {}
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
		return std::unique_ptr<Int>(new Int(value));
	}

	std::string to_string() const override {
		return std::to_string(value);
	}

private:
	int value;
};

#endif