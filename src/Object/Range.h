#ifndef RANGE_H
#define RANGE_H

#include "object/Float.h"

/**
 * Range is pretty simple object that contains min, max.
 *
 * It can return iterator to use it in for-loop.
 *
 * Now Range use Float for min, max because I don't want to create
 * one more Range for Float only, so now it can be used for Int and Float.
 *
 * Now there's no prefix and postfix range like `..b` or `a..`
 */

enum class RangeExclusion {
    Non, // `...`
    Left, // `>..`
    Right, // `..<`
    Both // `>.<`
};

class Range : public Object {
public:
	Range(float_ptr min, float_ptr max, RangeExclusion inclusion);
    Range(float_ptr max, RangeExclusion inclusion);
    virtual ~Range() = default;

    // TODO: Add default for max as +Infinity, after I implement Infinity
	
    std::string repr() const override;

private:
    float_ptr min;
    float_ptr max;
    RangeExclusion inclusion;
};

#endif