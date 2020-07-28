#include "object/Int.h"
#include "object/String.h"
#include "object/Float.h"
#include "object/Bool.h"
#include "object/Range.h"

Int::Int(yo_int i) : Object(ObjectType::Int, cInt), value(i)
{
    define_builtin("hash", make_nf(nullptr, "hash", {}, [this](NFArgs && args){
        return std::make_shared<Int>(static_cast<yo_int>(std::hash<yo_int>{}(value)));
    }));

    define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return std::make_shared<String>(std::to_string(value));
    }));

    define_builtin("__range", make_nf(nullptr, "__range", { {"to"} }, [this](NFArgs && args){
        int_ptr i_to = cast_to_i(args["to"]);

        if(!i_to){
            throw YoctoException("Invalid right-hand side in range `int...obj`");
        }

        return std::make_shared<Range>(shared_from_this(), i_to, RangeExcl::None);
    }));

    define_builtin("__range_le", make_nf(nullptr, "__range_le", { {"to"} }, [this](NFArgs && args){
        int_ptr i_to = cast_to_i(args["to"]);

        if(!i_to){
            throw YoctoException("Invalid right-hand side in range `int>..obj`");
        }

        return std::make_shared<Range>(shared_from_this(), i_to, RangeExcl::Left);
    }));

    define_builtin("__range_re", make_nf(nullptr, "__range_re", { {"to"} }, [this](NFArgs && args){
        int_ptr i_to = cast_to_i(args["to"]);

        if(!i_to){
            throw YoctoException("Invalid right-hand side in range `int..<obj`");
        }

        return std::make_shared<Range>(shared_from_this(), i_to, RangeExcl::Right);
    }));

    define_builtin("__range_bothe", make_nf(nullptr, "__range_bothe", { {"to"} }, [this](NFArgs && args){
        int_ptr i_to = cast_to_i(args["to"]);

        if(!i_to){
            throw YoctoException("Invalid right-hand side in range `int>.<obj`");
        }

        return std::make_shared<Range>(shared_from_this(), i_to, RangeExcl::Both);
    }));

    define_builtin("__eq", make_nf(nullptr, "__eq", { {"other"} }, [this](NFArgs && args){
        int_ptr i_to = cast_to_i(args["other"]);

        if(!i_to){
            throw YoctoException("Invalid right-hand side in range `int>.<obj`");
        }

        return make_bool(i_to->get_value() == value);
    }));
}

float_ptr Int::to_float() const {
    return std::make_shared<Float>((double)value);
}