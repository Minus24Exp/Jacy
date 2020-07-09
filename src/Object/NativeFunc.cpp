#include "object/NativeFunc.h"
#include "backend/Interpreter.h"

obj_ptr NativeFunc::call(Interpreter & ip, ObjList && args){
	NFArgs func_args;
	for(size_t i = 0; i < params.size(); i++){
		func_args.emplace(params[i].name, std::move(args[i]));
	}

	return body(std::move(func_args));
}