#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <vector>

class Object;
using obj_ptr = std::unique_ptr<Object>;
using ObjList = std::vector<obj_ptr>;

enum class ObjectType {
	Null,
	Bool,
	Int,
	Float,
	String,
	Callable
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
	virtual std::string to_string() const = 0;

};

#endif