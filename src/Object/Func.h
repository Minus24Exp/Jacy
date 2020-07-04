#ifndef FUNC_H
#define FUNC_H

#include "tree/FuncDecl.h"
#include "backend/Scope.h"
#include "Object/Object.h"
#include "Object/Callable.h"

class Func : public Callable {
public:
	Func(scope_ptr closure, FuncDecl & decl)
		: Callable(closure), decl(decl) {}
	
	virtual ~Func() = default;

	// Object //

	obj_ptr clone() const override {
		return std::unique_ptr<Func>(new Func(closure, decl));
	}

	std::string to_string() const override {
		return "<func:"+ decl.id->get_name() +">";
	}

	// Callable //
	size_t argc() const override {
		return decl.params.size();
	}

	std::string get_name() const override {
		return decl.id->get_name();
	}

	bool cmp_args(const ObjList & args) const override {
		if(args.size() != decl.params.size()){
			return false;
		}

		return true;
	}

	obj_ptr call(Interpreter & ip, ObjList && args) override;

private:
	FuncDecl & decl;
};

#endif