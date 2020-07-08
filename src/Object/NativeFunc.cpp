#include "object/NativeFunc.h"
#include "backend/Interpreter.h"

// TODO: !Important! capture `this` in native functions

obj_ptr NativeFunc::call(Interpreter & ip, ObjList && args){
	// For now I don't need to control scopes in NativeFunc
	// until the moment I implement Classes

	// scope_ptr closure_scope = std::make_shared<Scope>(closure);
	// scope_ptr previous = ip.get_scope();
	// ip.enter_scope(closure_scope);

	NFArgs func_args;
	for(size_t i = 0; i < params.size(); i++){
		func_args.emplace(params[i].name, std::move(args[i]));
	}

	obj_ptr ret_val = body(std::move(func_args));

	// ip.exit_scope();

	return ret_val;
}