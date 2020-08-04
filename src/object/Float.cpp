#include "object/Float.h"
#include "object/String.h"
#include "object/Int.h"
#include "object/Bool.h"
#include <cmath>

Float::Float(double d) : Object(ObjectType::Float, cFloat), value(d)
{   
    define_builtin("__hash", make_nf(nullptr, "__hash", {}, [this](NFArgs && args){
        return make_int(static_cast<yo_int>(std::hash<double>{}(value)));
    }));

    define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return make_string(std::to_string(value));
    }));

    define_builtin("__add", make_nf(nullptr, "__add", { {"r_term"} }, [this](NFArgs && args){
    	switch(args["r_term"]->get_obj_type()){
    		case ObjectType::Float:{
    			return make_float(value + s_cast_to_f(args["r_term"])->get_value());
    		}
    		case ObjectType::Int:{
    			return make_float(value + s_cast_to_i(args["r_term"])->get_value());
    		}
    		default: throw YoctoException("Invalid right-hand side in infix `+`");
    	}
    }));

    define_builtin("__sub", make_nf(nullptr, "__sub", { {"r_term"} }, [this](NFArgs && args){
    	switch(args["r_term"]->get_obj_type()){
    		case ObjectType::Float:{
    			return make_float(value - s_cast_to_f(args["r_term"])->get_value());
    		}
    		case ObjectType::Int:{
    			return make_float(value - s_cast_to_i(args["r_term"])->get_value());
    		}
    		default: throw YoctoException("Invalid right-hand side in infix `-`");
    	}
    }));

    define_builtin("__mul", make_nf(nullptr, "__mul", { {"r_term"} }, [this](NFArgs && args){
    	switch(args["r_term"]->get_obj_type()){
    		case ObjectType::Float:{
    			return make_float(value * s_cast_to_f(args["r_term"])->get_value());
    		}
    		case ObjectType::Int:{
    			return make_float(value * s_cast_to_i(args["r_term"])->get_value());
    		}
    		default: throw YoctoException("Invalid right-hand side in infix `*`");
    	}
    }));

    define_builtin("__div", make_nf(nullptr, "__div", { {"r_term"} }, [this](NFArgs && args){
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

    define_builtin("__pow", make_nf(nullptr, "__pow", { {"r_term"} }, [this](NFArgs && args){
        switch(args["r_term"]->get_obj_type()){
            case ObjectType::Float:{
                return make_float(pow(value, s_cast_to_f(args["r_term"])->get_value()));
            }
            case ObjectType::Int:{
                return make_float(pow(value, s_cast_to_i(args["r_term"])->get_value()));
            }
            default: throw YoctoException("Invalid right-hand side in infix `**`");
        }
    }));
    
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