#ifndef CLASS_H
#define CLASS_H

#include <memory>
#include "object/Callable.h"

class Class;
using class_ptr = std::shared_ptr<Class>;

// Note: Class does not need params because it compares args with 
// user-defined constructor

class Class : public Callable {
public:
	Class(scope_ptr closure, const std::string & name, class_ptr super)
		 : Callable(closure, name, {}), super(super) {}
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

private:
	// I don't think that classes could be copied
	// TODO: Think about it)
	obj_ptr clone() const override { return nullptr; }

	class_ptr super;
};

#endif