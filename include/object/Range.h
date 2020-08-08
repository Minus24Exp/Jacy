#ifndef RANGE_H
#define RANGE_H

#include "object/Object.h"

class Range;
extern std::shared_ptr<Class> cRange;
using range_ptr = std::shared_ptr<Range>;

class Int;
class Float;

enum class RangeExcl {
    None,   // `...`
    Left,   // `>..`
    Right,  // `..<`
    Both    // `>.<`
};

class Range : public Object {
public:
    Range(RangeExcl excl);
    virtual ~Range() = default;

    // Object //
    std::string repr() const override {
        return "<Range>";
    }

    virtual obj_ptr get_start() const = 0;
    virtual obj_ptr get_end() const = 0;

    // Get exclusiveness
    RangeExcl get_excl() const {
        return excl;
    }

private:
    RangeExcl excl;
};

#endif