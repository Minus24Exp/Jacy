#ifndef BASEFUNC_H
#define BASEFUNC_H

#include "object/Callable.h"

/**
 * BaseFunc - Base class for Func and NativeFunc
 */

struct Param {
    std::string name;
    obj_ptr default_val;

    Param(const std::string & name) : name(name), default_val(nullptr) {}
    Param(const std::string & name, obj_ptr dv) : name(name), default_val(dv) {}
};

using ParamList = std::vector<Param>;

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

    virtual obj_ptr call(Interpreter & ip, const ObjList & args) = 0;

    // BaseFunc //
    virtual obj_ptr bind(obj_ptr instance) = 0;

protected:
    scope_ptr closure;
    std::string name;
    ParamList params;

    size_t required_args_count;
};

#endif