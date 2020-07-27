#include "object/Range.h"
#include "object/String.h"

Range::Range(obj_ptr start, obj_ptr end, RangeExcl excl)
    : Object(ObjectType::Range, cRange), start(start), end(end), excl(excl)
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

        return std::make_shared<String>(str);
    }));
}