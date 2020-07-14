#include "Range.h"

Range::Range(float_ptr min, float_ptr max, RangeExclusion inclusion)
    : min(min), max(max), inclusion(inclusion)
{
    
}

std::string Range::repr() const {
    std::string str = "<Range:'";

    if(min){
        str += std::to_string(min->get_value());
    }

    switch(inclusion){
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

    if(max){
        str += std::to_string(max->get_value());
    }

    str += "'>";
    return str;
}