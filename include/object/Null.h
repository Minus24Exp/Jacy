#ifndef NULL_H
#define NULL_H

#include "object/NativeFunc.h"

class Null;
extern std::shared_ptr<Class> cNull;
extern std::shared_ptr<Null> null_obj;

class Null : public Object {
public:
    Null();
    virtual ~Null() = default;

    std::string repr() const override {
        return "<Null>";
    }
};

#endif