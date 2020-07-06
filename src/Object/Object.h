#ifndef OBJECT_H
#define OBJECT_H

#include <memory>
#include <vector>
#include <unordered_map>

class Object;
using obj_ptr = std::shared_ptr<Object>;
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

static inline std::string objtype2str(const ObjectType & type){
	switch(type){
		case ObjectType::Null: return "Null";
		case ObjectType::Bool: return "Bool";
		case ObjectType::Int: return "Int";
		case ObjectType::Float: return "Float";
		case ObjectType::String: return "String";
		case ObjectType::Callable: return "Callable";
		default: return "<unknown>";
	}
}

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
	virtual std::string to_string() const {
		return "<Object>";
	}

	Object * get(const std::string & name) const {
		// TODO: Add superclass

		if(fields.find(name) != fields.end()){
			return fields.at(name).get();
		}
		return nullptr;
	}

	void set(const std::string & name, obj_ptr obj){
		// TODO: Think about cases when field is not reassignable
		fields[name] = obj;
	}

protected:
	ObjFields fields;
};

#endif