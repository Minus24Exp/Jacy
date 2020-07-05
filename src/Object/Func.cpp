#include "object/Func.h"
#include "backend/Interpreter.h"


obj_ptr Func::call(Interpreter & ip, ObjList && args){
	ip.enter_scope(closure);

	for(size_t i = 0; i < params.size(); i++){
		if(i >= args.size() && params[i].default_val){
			ip.get_scope()->define(params[i].name, std::move(params[i].default_val));
		}else{
			ip.get_scope()->define(params[i].name, std::move(args[i]));
		}
	}

	obj_ptr return_val = make_null();

	try {
		ip.execute_block(body.get(), ip.get_scope());
	}catch(Object & val){
		// TODO: Add return value
		// return_val = std::make_unique<Object>(&val);
	}

	ip.exit_scope();

	return return_val;
}