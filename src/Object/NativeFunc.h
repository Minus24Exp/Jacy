#ifndef NATIVEFUNC_H
#define NATIVEFUNC_H

#include <functional>
#include <unordered_map>
#include "Object/Callable.h"

struct Param {
	std::string name;
};

using NFParams = std::vector<Param>;
using NFArgs = std::unordered_map<std::string, obj_ptr>;
using NFBody = std::function<obj_ptr(NFArgs&&)>;

class NativeFunc : public Callable {
public:
	NativeFunc(scope_ptr closure,
			   const std::string & name,
			   const NFParams & params,
			   const NFBody & body)
			  : Callable(closure), name(name), params(params), body(body) {}
	virtual ~NativeFunc() = default;

	// Object //
	obj_ptr clone() const override {
		return std::unique_ptr<NativeFunc>(new NativeFunc(closure, name, params, body));
	}

	std::string to_string() const override {
		return "<native_func:"+ name +">";
	}

	// Callable //
	size_t argc() const override {
		return params.size();
	}

	std::string get_name() const override {
		return name;
	}

	bool cmp_args(const ObjList & args) const override {
		if(args.size() != params.size()){
			return false;
		}

		return true;
	}

	obj_ptr call(Interpreter & interpreter, ObjList && args) override;

private:
	std::string name;
	NFParams params;
	NFBody body;
};

static inline obj_ptr make_nf(scope_ptr closure,
							  const std::string & name,
							  const NFParams & params,
							  const NFBody & body)
{
	return std::make_unique<NativeFunc>(closure, name, params, body);
}

#endif