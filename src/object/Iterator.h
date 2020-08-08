#ifndef ITERATOR_H
#define ITERATOR_H

#include "object/Object.h"

// Iterator //
template <typename T>
class Iterator : public Object {
public:
    Iterator();
    virtual ~Iterator() = default;

    // Object //
    virtual std::string repr() const override {
        return 
    }

    virtual bool has_next();
    virtual T next();
};

#endif