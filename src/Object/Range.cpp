#include "object/Range.h"

Range::Range(float_ptr start, float_ptr end, RangeExclusion exclusion)
    : start(start), end(end), exclusion(exclusion) {}

Range::Range(float_ptr end, RangeExclusion exclusion)
    : Range(std::make_shared<Float>(0.0), end, exclusion) {}

std::string Range::repr() const {
    std::string str = "<Range:'";

    if(start){
        str += std::to_string(start->get_value());
    }

    switch(exclusion){
        case RangeExclusion::Non:{
            str += "...";
            break;
        }
        case RangeExclusion::Left:{
            str += ">..";
            break;
        }
        case RangeExclusion::Right:{
            str += "..<";
            break;
        }
        case RangeExclusion::Both:{
            str += ">.<";
            break;
        }
    }

    if(end){
        str += std::to_string(end->get_value());
    }

    str += "'>";
    return str;
}

float_ptr Range::get_real_start() const {
    // Check for start exclusion
    if(exclusion == RangeExclusion::Left || exclusion == RangeExclusion::Both){
        return std::make_shared<Float>(start->get_value() + 1.0);
    }

    return start;
}

float_ptr Range::get_real_end() const {
    // Check for start exclusion
    if(exclusion == RangeExclusion::Right || exclusion == RangeExclusion::Both){
        return std::make_shared<Float>(end->get_value() - 1.0);
    }

    return end;
}