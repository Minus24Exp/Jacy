#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <vector>
#include <unordered_map>

#include "backend/Scope.h"

class Object;
using obj_ptr = std::unique_ptr<Object>;
using ObjList = std::vector<obj_ptr>;
using ObjFields = std::unordered_map<std::string, obj_ptr>;

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
	Object(const ObjectType & type);
	virtual ~Object() = default;

	ObjectType type;

	virtual bool truthy() const {
		return true;
	}

	virtual bool equals(Object * other) const = 0;
	virtual obj_ptr clone() const = 0;
	virtual std::string to_string() const = 0;

	Object * get_field(const std::string & name) const {
		if(fields.find(name) != fields.end()){
			return fields.at(name).get();
		}
		return nullptr;
	}

	void set_field(const std::string & name, obj_ptr obj){
		// TODO: Think about cases when field is not reassignable
		fields[name] = std::move(obj);
	}

private:
	ObjFields fields;
	scope_ptr scope;
};

#endif