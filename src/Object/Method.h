#ifndef METHOD_H
#define METHOD_H

// TODO: Add parameters types

#include "backend/Scope.h"
#include "Object/Object.h"
#include "Object/Callable.h"
#include "tree/FuncDecl.h"

#include <vector>

class Method : public Object, public Callable {
public:
	Method(FuncDecl & func_decl, Scope * closure) : decl(decl), closure(closure) {}
	virtual ~Method() = default;

	int argc() const {
		return decl.params.size();
	}

	Method * bind(Instance * instance){
		Scope * scope = new Scope(closure);
		scope->define("this", instance);

		return new Method(decl, scope);
	}

	Object * call(BaseVisitor & visitor, const Args & args) override {
		if(!compare_args(args)){
			throw YoctoException("Invalid parameters");
		}

		Scope * scope = new Scope(closure);

		for(int i = 0; i < decl.params.size(); i++){
			scope->define(decl.params[i].name, args[i]);
		}

		interpreter.visit(decl.body);
		
		// TODO: !!! Add return value
		return nullptr;
	}

private:
	Scope * closure;
	FuncDecl & decl;

	bool compare_args(const Args & args){
		if(args.size() != argc()){
			return false;
		}

		return true;
	}
};

#endif