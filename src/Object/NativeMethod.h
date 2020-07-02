#ifndef NATIVEMETHOD_H
#define NATIVEMETHOD_H

#include "Object/Callable.h"

#include <functional>
#include <map>

/**
 * NativeMethod differ from Method in the way it passes arguments
 * It creates map of names and objects that were received
 */

using NMArgs = std::map<std::string, Object*>;
using NativeMethodBody = std::function<Object*(const NMArgs & args)>;

class NativeMethod : public Callable {
public:
	NativeMethod(const Params & params, const NativeMethodBody & body)
		: body(body), Callable(params) {}
	virtual ~NativeMethod() = default;
	
	Object * call(BaseVisitor & visitor, const Args & args) override {
		compare_args(args);

		// TODO: variadic args
		// Idea: For variadic use argument "*" that will show that next arg is variadic

		NMArgs named_args;
		for(int i = 0; i < params.size(); i++){
			named_args[params[i].name] = args[i];
		}
		return body(named_args);
	}

private:
	NativeMethodBody body;
};

// Helpers
inline NativeMethod * make_native_method(const Params & params, const NativeMethodBody & body){
	return new NativeMethod(params, body);
}
inline NativeMethod * make_native_method(const NativeMethodBody & body){
	return new NativeMethod({}, body);
}

#endif