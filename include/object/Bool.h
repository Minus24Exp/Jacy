#ifndef BOOL_H
#define BOOL_H

#include "object/NativeFunc.h"

extern std::shared_ptr<Class> cBool;

class Bool : public Object {
public:
    Bool(bool b);
    virtual ~Bool() = default;

    // Object //
    bool truthy() const override {
        return value;
    }

    std::string repr() const override {
        return "<Bool:"+ std::string(value ? "true" : "false") +">";
    }

    bool get_value() const {
        return value;
    }

private:
    bool value;
};

#endif