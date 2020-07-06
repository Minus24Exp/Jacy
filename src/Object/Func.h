#ifndef FUNC_H
#define FUNC_H

#include "tree/FuncDecl.h"
#include "backend/Scope.h"
#include "object/Object.h"
#include "object/Callable.h"

class Func : public Callable {
public:
	Func(scope_ptr closure,
	     const std::string & name,
	     Params && params,
	     block_ptr body
	  	) : Callable(closure, name, std::move(params)),
	  		body(body) {}

	virtual ~Func() = default;

	// Object //
	obj_ptr clone() const override {
		// As params contains unique_ptr of default values I need to create a copy of it
		Params params_copy;
		params_copy.reserve(params.size());
		for(const auto & p : params){
			params_copy.push_back(Param(p.name, p.default_val ? p.default_val->clone() : nullptr));
		}
		return std::make_unique<Func>(closure, name, std::move(params_copy), body);
	}

	std::string to_string() const override {
		return "<func:"+ name +">";
	}

	// Callable //
	obj_ptr call(Interpreter & ip, ObjList && args) override;

private:
	block_ptr body;
};

#endif