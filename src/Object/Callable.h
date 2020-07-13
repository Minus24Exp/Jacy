#ifndef CALLABLE_H
#define CALLABLE_H

#include "object/Object.h"

/**
 * For now recursion depth limit is not tunable,
 * and I won't change it in the future, I think 1000 is pretty enough.
 * As far as C++ does not limit recursion depth, but just exit on stack
 * size excess, it's too hard to handle it, sooo...
 */

const int REC_DEPTH_MAX = 1000;

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
    
    // Update recursion info and check for errors //
    void update_recursion_depth(){
        recursion_depth++;
        if(recursion_depth > REC_DEPTH_MAX){
            throw RecursionDepthExceeded(REC_DEPTH_MAX);
        }
    }

    void reset_recursion_depth(){
        recursion_depth = 0;
    }

    virtual obj_ptr call(Interpreter & ip, const ObjList & args) = 0;

private:
    int recursion_depth = 0;
};

#endif