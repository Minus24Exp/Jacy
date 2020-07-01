#ifndef CALLABLE_H
#define CALLABLE_H

#include <vector>

class BaseVisitor;

/**
 * Callable is an abstract class for Method and NativeMethod.
 * It also can be used for Class to call constructor.
 */

class Object;

using Args = std::vector<Object*>;

// ParamStruct describe the structure of argument.
// It's will be really useful if there'll be gradual typization
struct ParamStruct {
	std::string name;
};

using Params = std::vector<ParamStruct>;

class Callable : public Object, public Scope {
public:
	Callable(const Params & params) : params(params) {}
	virtual ~Callable() = default;

	virtual Object * call(BaseVisitor & visitor, const Args & args) = 0;

	int argc(){
		return params.size();
	}

	Params get_params() const {
		return params;
	}

	bool compare_args(const Args & args) {
		if(args.size() != argc()){
			return false;
		}

		return true;
	}

protected:
	std::vector<ParamStruct> params;
};

#endif