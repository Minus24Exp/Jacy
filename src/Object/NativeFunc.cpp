#include "Object/NativeFunc.h"
#include "backend/Interpreter.h"

void NativeFunc::call(Interpreter & ip, ObjList && args){
	ip.enter_scope(closure);

	NFArgs func_args;
	for(size_t i = 0; i < params.size(); i++){
		func_args.emplace(params[i].name, std::move(args[i]));
	}

	body(std::move(func_args));
}