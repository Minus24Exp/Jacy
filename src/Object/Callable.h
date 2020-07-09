#ifndef CALLABLE_H
#define CALLABLE_H

/**
 * Callable is an abstract parent class for Func and NativeFunc.
 */

#include <vector>

#include "backend/Scope.h"
#include "object/Object.h"

class Callable;
using callable_ptr = std::shared_ptr<Callable>;

struct Param {
	std::string name;
	obj_ptr default_val;

	Param(const std::string & name) : name(name), default_val(nullptr) {}
	Param(const std::string & name, obj_ptr dv) : name(name), default_val(dv) {}
};

using Params = std::vector<Param>;

enum class CmpArgsResult {
	Ok,
	TooFew,
	TooMany
};

class Interpreter;

class Callable : public Object {
public:
	Callable(scope_ptr closure,
			 const std::string & name,
			 const Params & params
			) : Object(ObjectType::Callable),
		  		closure(closure),
		  		name(name),
		  		params(params)
	{
		// Set count of required arguments count (used by errors)
		required_argc = std::count_if(params.begin(), params.end(), [](const auto & p){
			if(p.default_val) return false;
			return true;
		});
	}

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
	
	// Get count of required arguments
	size_t get_required_argc() const {
		return required_argc;
	}

	size_t get_max_argc() const {
		return params.size();
	}

	std::string get_name() const {
		return name;
	}
	
	virtual CmpArgsResult cmp_args(const ObjList & args) const;

	virtual obj_ptr call(Interpreter & interpreter, ObjList && args) = 0;

protected:
	scope_ptr closure;
	std::string name;
	Params params;
	size_t required_argc;
};

#endif