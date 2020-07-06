#ifndef CALLABLE_H
#define CALLABLE_H

/**
 * Callable is an abstract parent class for Func and NativeFunc.
 */

#include <vector>

#include "backend/Scope.h"
#include "object/Object.h"

class Interpreter;

struct Param {
	std::string name;
	obj_ptr default_val;

	Param(const std::string & name) : name(name), default_val(nullptr) {}
	Param(const std::string & name, obj_ptr dv) : name(name), default_val(dv) {}
};

using Params = std::vector<Param>;

class Callable : public Object {
public:
	Callable(scope_ptr closure,
			 const std::string & name,
			 const Params & params
			) : Object(ObjectType::Callable),
		  		closure(closure),
		  		name(name),
		  		params(params) {}

	virtual ~Callable() = default;

	// Object //
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
	std::string get_name() const {
		return name;
	}
	
	bool cmp_args(const ObjList & args) const;

	virtual obj_ptr call(Interpreter & interpreter, ObjList && args) = 0;

protected:
	scope_ptr closure;
	std::string name;
	Params params;
};

#endif