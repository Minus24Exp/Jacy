#ifndef RANGE_H
#define RANGE_H

#include "object/Object.h"

class Range;
using range_ptr = std::shared_ptr<Range>;

extern std::shared_ptr<Class> cRange;

enum class RangeExcl {
    None,   // `...`
    Left,   // `>..`
    Right,  // `..<`
    Both    // `>.<`
};

class Range : public Object {
public:
    Range(obj_ptr start, obj_ptr end, RangeExcl excl);
    virtual ~Range() = default;

    // Object //
    std::string repr() const override {
        return "<Range>";
    }

    obj_ptr get_start() const;
    obj_ptr get_end() const;

private:
    obj_ptr start;
    obj_ptr end;
    RangeExcl excl;
};

#endif