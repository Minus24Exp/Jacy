#ifndef CALLABLE_H
#define CALLABLE_H

#include "object/Object.h"

enum class CmpArgsResult {
	Ok,
	TooFew,
	TooMany
};

struct Param {
	std::string name;
	obj_ptr default_val;

	Param(const std::string & name) : name(name), default_val(nullptr) {}
	Param(const std::string & name, obj_ptr dv) : name(name), default_val(dv) {}
};

using Params = std::vector<Param>;

class Interpreter;

class Callable {
public:
	Callable(){}
	virtual ~Callable() = default;

	virtual size_t get_required_argc() const = 0;
	virtual size_t get_max_argc() const = 0;

	// Default cmp_args can be overriden
	virtual CmpArgsResult cmp_args(const ObjList & args) const {
		if(args.size() < get_required_argc()){
			return CmpArgsResult::TooFew;
		}

		if(args.size() > get_max_argc()){
			return CmpArgsResult::TooMany;
		}
		
		return CmpArgsResult::Ok;
	}

	virtual obj_ptr call(Interpreter & ip, ObjList && args) = 0;
};

#endif