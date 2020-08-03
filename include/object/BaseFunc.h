#ifndef BASEFUNC_H
#define BASEFUNC_H

#include "object/Callable.h"
#include "tree/Stmt/FuncDecl.h"

/**
 * BaseFunc - Base class for Func and NativeFunc
 */

class BaseFunc;
using func_ptr = std::shared_ptr<BaseFunc>;

const auto cast_to_func = [](obj_ptr obj){
    return std::dynamic_pointer_cast<BaseFunc>(obj);
};

struct Param {
    std::string name;
    obj_ptr default_val;

    Param(const std::string & name) : name(name), default_val(nullptr) {}
    Param(const std::string & name, obj_ptr dv) : name(name), default_val(dv) {}
};

using ParamList = std::vector<Param>;

extern std::shared_ptr<Class> cFunc;

class BaseFunc : public Object, public Callable {
public:
    BaseFunc(scope_ptr closure,
             const std::string & name,
             const ParamList & params);
    virtual ~BaseFunc() = default;

    // Object //
    bool truthy() const override;

    virtual std::string repr() const override;

    // Callable //
    size_t required_argc() const override;
    size_t argc() const override;

    virtual obj_ptr call(const ObjList & args) = 0;

    // Helper for no args //
    obj_ptr call(){
        return call({});
    }

    // BaseFunc //
    virtual func_ptr bind(obj_ptr instance) = 0;

    bool is_init() const {
        return initializer;
    }

protected:
    scope_ptr closure;
    std::string name;
    ParamList params;

    bool initializer;

    size_t required_args_count;
};

#endif