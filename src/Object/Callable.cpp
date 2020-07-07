#include "object/Callable.h"
#include <iostream>

CmpArgsResult Callable::cmp_args(const ObjList & args) const {
	if(args.size() < required_argc){
		return CmpArgsResult::TooFew;
	}

	if(args.size() > params.size()){
		return CmpArgsResult::TooMany;
	}

	// for(size_t i = 0; i < params.size(); i++){
	// 	if(i >= params.size()){
	// 		return CmpArgsResult::TooMany;
	// 	}

	// 	if(i >= args.size() && !params[i].default_val){
	// 		return CmpArgsResult::TooFew;
	// 	}
	// }

	// Return empty string of no error occured,
	// `{}` because of little performance better hand than `""`
	return CmpArgsResult::Ok;
}