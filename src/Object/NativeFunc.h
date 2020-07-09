#ifndef NATIVEFUNC_H
#define NATIVEFUNC_H

#include <functional>
#include <unordered_map>
#include "object/Object.h"
#include "object/Callable.h"
#include "backend/Scope.h"
#include "object/Instance.h"

class NativeFunc;
using nf_ptr = std::shared_ptr<NativeFunc>;

using NFArgs = std::unordered_map<std::string, obj_ptr>;
using NFBody = std::function<obj_ptr(NFArgs && args)>;

class NativeFunc : public Object, public Callable {
public:
	NativeFunc(scope_ptr closure,
			   const std::string & name,
			   const Params & params,
			   const NFBody & body
			  ) : Object(ObjectType::NativeFunc),
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
	virtual ~NativeFunc() = default;

	// Object //
	bool equals(Object * other) const override {
		if(other->type != ObjectType::NativeFunc){
			return false;
		}
		return true;
	}

	obj_ptr clone() const override {
		return std::make_shared<NativeFunc>(closure, name, params, body);
	}

	std::string to_string() const override {
		return "<native_func:"+ name +">";
	}

	// Callable //
	size_t get_required_argc() const override {
		return required_argc;
	}

	size_t get_max_argc() const override {
		return params.size();
	}

	obj_ptr call(Interpreter & ip, ObjList && args) override;

	// NativeFunc //
	std::string get_name() const {
		return name;
	}

	nf_ptr bind(instance_ptr instance){
		scope_ptr nf_scope = std::make_shared<Scope>(closure);
		nf_scope->define("this", {LocalDeclType::Val, instance});
		return std::make_shared<NativeFunc>(nf_scope, name, params, body);
	}

private:
	scope_ptr closure;
	std::string name;
	Params params;
	NFBody body;

	size_t required_argc;
};

static inline nf_ptr make_nf(scope_ptr closure,
			   const std::string & name,
			   const Params & params,
			   const NFBody & body)
{
	return std::make_shared<NativeFunc>(closure, name, params, body);
}

#endif