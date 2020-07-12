#ifndef CALLABLE_H
#define CALLABLE_H

#include "object/Object.h"

class Interpreter;

/**
 * Callable - Abstract class for callable objects
 *
 * required_argc - get count of parameters without default value
 * argc - get count of all parameters (max params expected)
 */

enum class ArgsCmpResult {
    Ok,
    TooFew,
    TooMany
};

class Callable {
public:
    virtual ~Callable() = default;

    virtual size_t required_argc() const = 0;
    virtual size_t argc() const = 0;

    virtual ArgsCmpResult cmp_args(const ObjList & args) const {
        if(args.size() < required_argc()){
            return ArgsCmpResult::TooFew;
        }

        if(args.size() > argc()){
            return ArgsCmpResult::TooMany;
        }
        
        return ArgsCmpResult::Ok;
    }

    virtual obj_ptr call(Interpreter & ip, const ObjList & args) = 0;
};

#endif