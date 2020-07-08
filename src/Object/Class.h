#ifndef CLASS_H
#define CLASS_H

#include <memory>
#include <unordered_map>
#include "object/Callable.h"

class Class;
using class_ptr = std::shared_ptr<Class>;

// Note: Class does not need params because it compares args with 
// user-defined constructor

class Class : public Callable, public std::enable_shared_from_this<Class> {
public:
	Class(scope_ptr closure, const std::string & name, class_ptr super, const LocalMap & fields)
		 : Callable(closure, name, {}), super(super), fields(fields) {}
	virtual ~Class() = default;

	// Object //
	bool equals(Object * other) const override {
		// TODO: Make real equality comparison

		return false;
	}

	std::string to_string() const {
		return "<class:"+ name +">";
	}

	// Callable //
	CmpArgsResult cmp_args(const ObjList & args) const override {
		// TODO: User-defined constructors cmp_args
		return CmpArgsResult::Ok;
	}

	obj_ptr call(Interpreter & ip, ObjList && args) override;

	// Class //
	bool has(const std::string & name) const {
		if(fields.find(name) != fields.end()){
			return true;
		}

		if(super){
			return super->has(name);
		}

		return false;
	}

	obj_ptr get(const std::string & name) const {
		if(fields.find(name) != fields.end()){
			return fields.at(name).val;
		}

		if(super){
			return super->get(name);
		}

		return nullptr;
	}

	// TODO: Add statics

	// void set_static(const std::string & name, obj_ptr value){}

private:
	// I don't think that class may be copied
	// TODO: Think about it ;)
	obj_ptr clone() const override { return nullptr; }

	// Class constructor cannot be bind to instance, i think...
	// TODO: Think about it
	obj_ptr bind(obj_ptr instance) override { return nullptr; }

	LocalMap fields;
	class_ptr super;
};

#endif