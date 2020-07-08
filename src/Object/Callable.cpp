#include "object/Callable.h"

CmpArgsResult Callable::cmp_args(const ObjList & args) const {
	if(args.size() < required_argc){
		return CmpArgsResult::TooFew;
	}

	if(args.size() > params.size()){
		return CmpArgsResult::TooMany;
	}
	
	return CmpArgsResult::Ok;
}