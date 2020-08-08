#include "object/Range.h"
#include "object/Int.h"
#include "object/Float.h"
#include "object/String.h"

Range::Range(RangeExcl excl)
    : Object(ObjectType::Range, cRange), excl(excl)
{
    // define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
    //     std::string str;

    //     if(this->start){
    //         str += obj_to_str(this->start);
    //     }

    //     switch(this->excl){
    //         case RangeExcl::None:{
    //             str += "...";
    //         } break;
    //         case RangeExcl::Left:{
    //             str += ">..";
    //         } break;
    //         case RangeExcl::Right:{
    //             str += "..<";
    //         } break;
    //         case RangeExcl::Both:{
    //             str += ">.<";
    //         } break;
    //     }

    //     if(this->end){
    //         str += obj_to_str(this->end);
    //     }

    //     return make_string(str);
    // }));
}