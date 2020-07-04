#ifndef STRING_H
#define STRING_H

#include "Object/Object.h"

class String : public Object {
public:
	String(scope_ptr closure, const std::string & s);
	virtual ~String() = default;

	std::string get_value() const {
		return value;
	}

	bool equals(Object * other) const override {
		if(other->type != ObjectType::String){
			return false;
		}
		return value == static_cast<String*>(other)->get_value();
	}

	obj_ptr clone() const override {
		return std::unique_ptr<String>(new String(closure, value));
	}

	std::string to_string() const override {
		return "<String:'"+ value +"'>";
	}

private:
	std::string value;
};

#endif