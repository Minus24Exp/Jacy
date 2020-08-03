#include "object/Float.h"
#include "object/String.h"
#include "object/Int.h"

Float::Float(double d) : Object(ObjectType::Float, cFloat), value(d)
{   
    define_builtin("__hash", make_nf(nullptr, "__hash", {}, [this](NFArgs && args){
        return make_int(static_cast<yo_int>(std::hash<double>{}(value)));
    }));

    define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return make_string(std::to_string(value));
    }));

    // define_builtin("__add", make_nf(nullptr, "__add", { {"r_term"} }, [this](NFArgs && args){
    // 	switch(args["r_term"]->get_obj_type()){
    // 		case ObjectType::Float:{
    // 			return make_float(value + cast_to_f(args["r_term"])->get_value());
    // 		}
    // 		case ObjectType::Int:{
    // 			return make_float(value + cast_to_i(args["r"]))
    // 		}
    // 	}
    // }));
}