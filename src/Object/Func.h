#ifndef FUNC_H
#define FUNC_H

/**
 * Function is an abstract parent class for Method and NativeMethod.
 */

#include <vector>

#include "tree/Stmt/FuncDecl.h"
#include "backend/Scope.h"
#include "object/Callable.h"
#include "object/Instance.h"

class Func;
using func_ptr = std::shared_ptr<Func>;

class Func : public Object, public Callable {
public:
	Func(scope_ptr closure,
		 const std::string & name,
		 const Params & params,
		 block_ptr body
		) : Object(ObjectType::Func),
		  	closure(closure),
		  	name(name),
		  	params(params),
		  	body(body)
	{
		// Set count of required arguments count (used by errors)
		required_argc = std::count_if(params.begin(), params.end(), [](const auto & p){
			if(p.default_val) return false;
			return true;
		});
	}

	virtual ~Func() = default;

	// Object //
	bool equals(Object * other) const override {
		if(other->type != ObjectType::Func){
			return false;
		}
		return true;
	}

	obj_ptr clone() const override {
		return std::make_shared<Func>(closure, name, params, body);
	}

	std::string to_string() const override {
		return "<func:"+ name +">";
	}

	// Callable //
	size_t get_required_argc() const override {
		return required_argc;
	}

	size_t get_max_argc() const override {
		return params.size();
	}

	obj_ptr call(Interpreter & ip, ObjList && args) override;

	// Func //
	std::string get_name() const {
		return name;
	}

	func_ptr bind(instance_ptr instance){
		scope_ptr func_scope = std::make_shared<Scope>(closure);
		func_scope->define("this", {LocalDeclType::Val, instance});
		return std::make_shared<Func>(func_scope, name, params, body);
	}

protected:
	scope_ptr closure;
	std::string name;
	Params params;
	block_ptr body;

	// Required argc is helper for cmp_args that stores count of params without default value
	size_t required_argc;
};

#endif