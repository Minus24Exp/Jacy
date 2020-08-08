#include "object/Range.h"
#include "object/Int.h"
#include "object/Float.h"
#include "object/String.h"

///////////
// Range //
///////////
Range::Range(RangeExcl excl) : Object(ObjectType::Range, cRange), excl(excl) {}

//////////////
// IntRange //
//////////////
IntRange::IntRange(std::shared_ptr<Int> start, std::shared_ptr<Int> end, RangeExcl excl)
    : Range(excl), start(start), end(end)
{
    define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        std::string str;

        if(this->start){
            str += obj_to_str(this->start);
        }

        switch(this->excl){
            case RangeExcl::None:{
                str += "...";
            } break;
            case RangeExcl::Left:{
                str += ">..";
            } break;
            case RangeExcl::Right:{
                str += "..<";
            } break;
            case RangeExcl::Both:{
                str += ">.<";
            } break;
        }

        if(this->end){
            str += obj_to_str(this->end);
        }

        return make_string(str);
    }));
}

std::string IntRange::repr() const override {
    // Here `obj_to_str` will exactly call `to_s`
    return "<IntRange:"+ obj_to_str(shared_from_this()) +">";
}

yo_int IntRange::get_start() const {
    return start;
}

yo_int IntRange::get_end() const {
    return end;
}