#include "object/Int.h"
#include "object/String.h"
#include "object/Float.h"
#include "object/Bool.h"
#include <cmath>

Int::Int(yo_int i) : Object(ObjectType::Int, cInt), value(i)
{
    define_builtin("__hash", make_nf(nullptr, "__hash", {}, [this](NFArgs && args){
        return make_int(static_cast<yo_int>(std::hash<yo_int>{}(value)));
    }));

    define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return make_string(std::to_string(value));
    }));

    define_builtin("__add", make_nf(nullptr, "__add", { {"r_term"} }, [this](NFArgs && args) -> obj_ptr {
        switch(args["r_term"]->get_obj_type()){
            case ObjectType::Float:{
                return make_float(value + s_cast_to_f(args["r_term"])->get_value());
            }
            case ObjectType::Int:{
                return make_int(value + s_cast_to_i(args["r_term"])->get_value());
            }
            default: throw YoctoException("Invalid right-hand side in infix `+`");
        }
    }));

    define_builtin("__sub", make_nf(nullptr, "__sub", { {"r_term"} }, [this](NFArgs && args) -> obj_ptr {
        switch(args["r_term"]->get_obj_type()){
            case ObjectType::Float:{
                return make_float(value - s_cast_to_f(args["r_term"])->get_value());
            }
            case ObjectType::Int:{
                return make_int(value - s_cast_to_i(args["r_term"])->get_value());
            }
            default: throw YoctoException("Invalid right-hand side in infix `-`");
        }
    }));

    define_builtin("__mul", make_nf(nullptr, "__mul", { {"r_term"} }, [this](NFArgs && args) -> obj_ptr {
        switch(args["r_term"]->get_obj_type()){
            case ObjectType::Float:{
                return make_float(value * s_cast_to_f(args["r_term"])->get_value());
            }
            case ObjectType::Int:{
                return make_int(value * s_cast_to_i(args["r_term"])->get_value());
            }
            default: throw YoctoException("Invalid right-hand side in infix `*`");
        }
    }));

    define_builtin("__div", make_nf(nullptr, "__div", { {"r_term"} }, [this](NFArgs && args) -> obj_ptr {
        switch(args["r_term"]->get_obj_type()){
            case ObjectType::Float:{
                return make_float(value / s_cast_to_f(args["r_term"])->get_value());
            }
            case ObjectType::Int:{
                return make_float(value / s_cast_to_i(args["r_term"])->get_value());
            }
            default: throw YoctoException("Invalid right-hand side in infix `/`");
        }
    }));

    define_builtin("__pow", make_nf(nullptr, "__pow", { {"r_term"} }, [this](NFArgs && args) -> obj_ptr {
        switch(args["r_term"]->get_obj_type()){
            case ObjectType::Float:{
                return make_float(pow(value, s_cast_to_f(args["r_term"])->get_value()));
            }
            case ObjectType::Int:{
                return make_int(pow(value, s_cast_to_i(args["r_term"])->get_value()));
            }
            default: throw YoctoException("Invalid right-hand side in infix `**`");
        }
    }));

    // define_builtin("__range", make_nf(nullptr, "__range", { {"to"} }, [this](NFArgs && args){
    //     if(args["to"]->get_obj_type() != ObjectType::Int){
    //         throw YoctoException("Invalid right-hand side in range `int...obj`");
    //     }

    //     return std::make_shared<Range>(shared_from_this(), s_cast_to_i(args["to"]), RangeExcl::None);
    // }));

    // define_builtin("__range_le", make_nf(nullptr, "__range_le", { {"to"} }, [this](NFArgs && args){
    //     if(args["to"]->get_obj_type() != ObjectType::Int){
    //         throw YoctoException("Invalid right-hand side in range `int>..obj`");
    //     }

    //     return std::make_shared<Range>(shared_from_this(), s_cast_to_i(args["to"]), RangeExcl::Left);
    // }));

    // define_builtin("__range_re", make_nf(nullptr, "__range_re", { {"to"} }, [this](NFArgs && args){
    //     if(args["to"]->get_obj_type() != ObjectType::Int){
    //         throw YoctoException("Invalid right-hand side in range `int..<obj`");
    //     }

    //     return std::make_shared<Range>(shared_from_this(), s_cast_to_i(args["to"]), RangeExcl::Right);
    // }));

    // define_builtin("__range_bothe", make_nf(nullptr, "__range_bothe", { {"to"} }, [this](NFArgs && args){
    //     if(args["to"]->get_obj_type() != ObjectType::Int){
    //         throw YoctoException("Invalid right-hand side in range `int>.<obj`");
    //     }

    //     return std::make_shared<Range>(shared_from_this(), s_cast_to_i(args["to"]), RangeExcl::Both);
    // }));

    define_builtin("__eq", make_nf(nullptr, "__eq", { {"other"} }, [this](NFArgs && args){
        switch(args["other"]->get_obj_type()){
            case ObjectType::Float:{
                return make_bool(value == s_cast_to_f(args["other"])->get_value());
            }
            case ObjectType::Int:{
                return make_bool(value == s_cast_to_i(args["other"])->get_value());
            }
        }
        return false_obj;
    }));
}

float_ptr Int::to_float() const {
    return make_float((double)value);
}