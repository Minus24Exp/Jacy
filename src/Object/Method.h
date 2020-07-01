#ifndef METHOD_H
#define METHOD_H

// TODO: Add parameters types

#include "backend/Scope.h"
#include "Object/Object.h"
#include "Object/Callable.h"
#include "tree/FuncDecl.h"

#include <vector>

class Method : public Callable {
public:
	Method(FuncDecl & func_decl) : Callable({}) {
		for(const auto & param : func_decl.params){
			params.push_back({ .name = param.id.get_name() });
		}
		body = func_decl.body.stmts;
	}
	virtual ~Method() = default;

	Object * call(BaseVisitor & visitor, const Args & args) override {
		compare_args(args);

		for(int i = 0; i < params.size(); i++){
			define(params[i].name, {args[i]});
		}

		for(Statement * stmt : body){
			stmt->accept(visitor);
		}

		// TODO: !!! Add return statement and return last value if there's no return
		return nullptr;
	}

private:
	StatementList body;
};

#endif