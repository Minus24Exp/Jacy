#ifndef FUNC_H
#define FUNC_H

#include "tree/FuncDecl.h"
#include "backend/Scope.h"
#include "Object/Object.h"

class Func : public Object {
public:
	Func(FuncDecl & decl, scope_ptr closure)
		: Object(ObjectType::Func),
		  decl(decl),
		  closure(closure) {}
	
	virtual ~Func() = default;

	bool truthy() const override {
		return true;
	}

	bool equals(Object * other) const override {
		return other->type == ObjectType::Func;
	}

	obj_ptr clone() const override {
		return std::unique_ptr<Func>(new Func(decl, closure));
	}

	std::string to_string() const override {
		return "<func"+ decl.id->get_name() +">";
	}

	size_t argc(){
		return decl.params.size();
	}

	FuncDecl & decl;
	scope_ptr closure;

};

#endif