#ifndef NATIVEFUNC_H
#define NATIVEFUNC_H

#include <unordered_map>
#include <functional>
#include "object/BaseFunc.h"

using NFArgs = std::unordered_map<std::string, obj_ptr>;
using NFBody = std::function<obj_ptr(NFArgs && args)>;

class NativeFunc : public BaseFunc {
public:
    NativeFunc(scope_ptr closure,
               const std::string & name,
               const ParamList & params,
               const NFBody & body);
    virtual ~NativeFunc() = default;

    // Object //
    std::string repr() const override;

    // BaseFunc //
    obj_ptr call(const ObjList & args) override;
    obj_ptr bind(scope_ptr closure, obj_ptr instance) override;

private:
    NFBody body;
};

static inline std::shared_ptr<NativeFunc> make_nf(scope_ptr closure,
                                                  const std::string & name,
                                                  const ParamList & params,
                                                  const NFBody & body)
{
    return std::make_shared<NativeFunc>(closure, name, params, body);
}

#endif