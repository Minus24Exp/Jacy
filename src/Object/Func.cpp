#include "Object/Func.h"
#include "backend/Interpreter.h"

obj_ptr Func::call(Interpreter & ip, ObjList && args){
	ip.enter_scope(closure);

	for(size_t i = 0; i < decl.params.size(); i++){
		ip.get_scope()->define(decl.params[i].id->get_name(), std::move(args[i]));
	}

	obj_ptr return_val = make_null();

	try {
		ip.execute_block(decl.body.get(), ip.get_scope());
	}catch(Object & val){
		// TODO: Add return value
		// return_val = std::make_unique<Object>(&val);
	}

	ip.exit_scope();

	return return_val;
}