#ifndef CALLABLE_H
#define CALLABLE_H

/**
 * Callable is an abstract parent class for Func and NativeFunc.
 */

#include <vector>

#include "backend/Scope.h"
#include "Object/Object.h"

class Interpreter;

class Callable : public Object {
public:
	Callable(scope_ptr closure)
			: Object(closure, ObjectType::Callable) {}

	virtual ~Callable() = default;

	// Object //
	bool truthy() const override {
		return true;
	}

	bool equals(Object * other) const override {
		if(other->type != ObjectType::Callable){
			return false;
		}

		// TODO: add pointer check

		return get_name() == static_cast<Callable*>(other)->get_name();
	}

	virtual obj_ptr clone() const = 0;

	virtual std::string to_string() const = 0;

	// Callable //
	virtual std::string get_name() const = 0;

	virtual size_t argc() const = 0;

	virtual bool cmp_args(const ObjList & args) const = 0;

	virtual obj_ptr call(Interpreter & interpreter, ObjList && args) = 0;
};

#endif