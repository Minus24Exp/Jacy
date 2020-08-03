#include "object/BaseFunc.h"

BaseFunc::BaseFunc(scope_ptr closure,
                   const std::string & name,
                   const ParamList & params,
                   FuncMode mode
                  ) : Object(ObjectType::Func, cFunc),
                      closure(closure),
                      name(name),
                      params(params),
                      mode(mode)
{
    // Count arguments without default value
    required_args_count = std::count_if(params.begin(), params.end(), [](const auto & p){
        if(p.default_val) return false;
        return true;
    });
}

bool BaseFunc::truthy() const {
    return true;
}

std::string BaseFunc::repr() const {
    return "<func:"+ name +">";
}

size_t BaseFunc::required_argc() const {
    return required_args_count;
}

size_t BaseFunc::argc() const {
    return params.size();
}