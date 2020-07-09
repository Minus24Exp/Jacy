#include "object/NativeFunc.h"
#include "backend/Interpreter.h"

obj_ptr NativeFunc::call(Interpreter & ip, ObjList && args){
	NFArgs func_args;
	for(size_t i = 0; i < params.size(); i++){
		func_args.emplace(params[i].name, std::move(args[i]));
	}

	obj_ptr ret_val = body(std::move(func_args));

	if(!ret_val){
		// Note: This is just a helper for built-in functions
		// They can return nullptr, and then here it will be converted to Null.
		// But, nullptr does not equal to Null
		ret_val = null_obj;
	}

	return ret_val;
}