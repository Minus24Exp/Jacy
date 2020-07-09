#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <vector>
#include <unordered_map>

class Object;
using obj_ptr = std::shared_ptr<Object>;
using ObjList = std::vector<obj_ptr>;

enum class ObjectType {
	Null,
	Bool,
	Int,
	Float,
	String,
	Func,
	NativeFunc,
	Class,
	Instance
};

class Object {
public:
	Object(const ObjectType & type) : type(type) {}
	virtual ~Object() = default;

	ObjectType type;

	virtual bool truthy() const {
		return true;
	}

	virtual bool equals(Object * other) const = 0;
	virtual obj_ptr clone() const = 0;

	// Note: to_string is not the same as `to_s`
	// it's used by error handlers and etc.
	// Maybe, if I'll implement some kind of `repr`
	// function like in Pyhton, then it will be
	// used there
	virtual std::string to_string() const {
		return "<Object>";
	}

};

#endif