#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "object/Object.h"

//////////
// Null //
//////////
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
		return std::make_shared<Null>();
	}

	std::string to_string() const override {
		return "<Null>";
	}
};

// Note: Null is not singleton itself
// But NEVER create new Null, always use null_obj

const auto null_obj = std::make_shared<Null>();

//////////
// Bool //
//////////
class Bool : public Object {
public:
	Bool(const bool & b) : Object(ObjectType::Bool), value(b) {}
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
		return std::make_shared<Bool>(value);
	}

	std::string to_string() const override {
		return "<Bool:"+ std::to_string(value) + ">";
	}

private:
	bool value;
};

/////////
// Int //
/////////
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
		return std::make_shared<Int>(value);
	}

	std::string to_string() const override {
		return "<Int:"+ std::to_string(value) +">";
	}

private:
	int value;
};

///////////
// Float //
///////////
class Float : public Object {
public:
	Float(const double & d) : Object(ObjectType::Float), value(d) {}
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
		return std::make_shared<Float>(value);
	}

	std::string to_string() const override {
		return "<Float:"+ std::to_string(value) + ">";
	}

private:
	double value;
};

////////////
// String //
////////////
class String : public Object {
public:
	String(const std::string & s) : Object(ObjectType::String), value(s) {}
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
		return std::make_shared<String>(value);
	}

	std::string to_string() const override {
		return "<String:'"+ value +"'>";
	}

private:
	std::string value;
};

#endif