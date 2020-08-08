// #ifndef RANGE_H
// #define RANGE_H

// #include "object/Object.h"

// class Range;
// extern std::shared_ptr<Class> cRange;
// using range_ptr = std::shared_ptr<Range>;

// class Int;
// class Float;

// enum class RangeExcl {
//     None,   // `...`
//     Left,   // `>..`
//     Right,  // `..<`
//     Both    // `>.<`
// };

// // Range //
// class Range : public Object {
// public:
//     Range(RangeExcl excl);
//     virtual ~Range() = default;

//     // Object //
//     virtual std::string repr() const override {
//         return "<Range>";
//     }

//     // Range //
    
//     // Get exclusiveness
//     RangeExcl get_excl() const {
//         return excl;
//     }

// private:
//     RangeExcl excl;
// };

// // IntRange //
// class IntRange : public Range {
// public:
//     IntRange(std::shared_ptr<Int> start, std::shared_ptr<Int> end, RangeExcl excl);
//     virtual ~IntRange() = default;

//     // Object //
//     std::string repr() const override;

//     // IntRange //
//     yo_int get_first() const;
//     yo_int get_last() const;

// private:
//     yo_int first;
//     yo_int last;
// };

// #endif