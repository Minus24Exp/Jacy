#ifndef RANGE_H
#define RANGE_H

#include "object/Float.h"

/**
 * Range is pretty simple object that contains start, end.
 *
 * It can return iterator to use it in for-loop.
 *
 * Now Range use Float for start, end because I don't want to create
 * one more Range for Float only, so now it can be used for Int and Float.
 *
 * Now there's no prefix and postfix range like `..b` or `a..`
 */

class Range;
using range_ptr = std::shared_ptr<Range>;

const auto cast_to_range = [](obj_ptr obj) { return std::dynamic_pointer_cast<Range>(obj); };

enum class RangeExclusion {
    Non, // `...`
    Left, // `>..`
    Right, // `..<`
    Both // `>.<`
};

class Range : public Object {
public:
	Range(float_ptr start, float_ptr end, RangeExclusion exclusion);
    Range(float_ptr end, RangeExclusion exclusion);
    virtual ~Range() = default;

    // TODO: Add default for end as +Infinity, after I implement Infinity
	
    std::string repr() const override;

    // Range //
    
    // get real start/end return numbers depending on exclusion
    float_ptr get_real_start() const;
    float_ptr get_real_end() const;

private:
    float_ptr start;
    float_ptr end;
    RangeExclusion exclusion;
};

#endif