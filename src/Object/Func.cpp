#include "Object/Func.h"
#include "backend/Interpreter.h"

void Func::call(Interpreter & ip, ObjList && args){
	ip.enter_scope(closure);

	for(size_t i = 0; i < decl.params.size(); i++){
		ip.get_scope()->define(decl.params[i].id->get_name(), std::move(args[i]));
	}

	try {
		ip.execute_block(decl.body.get(), ip.get_scope());
	}catch(Object & ret_val){
		// TODO: Add return value
	}
	
	ip.exit_scope();
}