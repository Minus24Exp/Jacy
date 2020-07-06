#include "object/Callable.h"
#include <iostream>

bool Callable::cmp_args(const ObjList & args) const {

	for(int i = 0; i < params.size(); i++){
		// Check if there's parameter without
		// default value that does not receive argument
		if(i >= args.size() && !params[i].default_val){
			return false;
		}
	}

	return true;
}