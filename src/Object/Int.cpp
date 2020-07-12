#include "object/Int.h"
#include "object/String.h"
#include "object/Float.h"
#include "object/Bool.h"

Int::Int(yo_int i) : value(i) {
    define_nf("__add", make_nf(nullptr, "__add", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<Int> other_i = cast_to_i(args["other"]);

        if(!other_i){
            throw 1;
        }

        return std::make_shared<Int>(value + other_i->get_value());
    }));

    define_nf("__sub", make_nf(nullptr, "__sub", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<Int> other_i = cast_to_i(args["other"]);

        if(!other_i){
            throw 1;
        }

        return std::make_shared<Int>(value - other_i->get_value());
    }));

    define_nf("__mul", make_nf(nullptr, "__mul", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<Int> other_i = cast_to_i(args["other"]);

        if(!other_i){
            throw 1;
        }

        return std::make_shared<Int>(value * other_i->get_value());
    }));

    define_nf("__div", make_nf(nullptr, "__div", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<Int> other_i = cast_to_i(args["other"]);

        if(!other_i){
            throw "Invalid right-hand side";
        }

        return std::make_shared<Float>((double)value / (double)other_i->get_value());
    }));

    define_nf("__mod", make_nf(nullptr, "__mod", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<Int> other_i = cast_to_i(args["other"]);

        if(!other_i){
            throw 1;
        }

        return std::make_shared<Int>(value % other_i->get_value());
    }));

    define_nf("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return std::make_shared<String>(std::to_string(value));
    }));

    define_nf("__eq", make_nf(nullptr, "__eq", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<Int> other_i = cast_to_i(args["other"]);

        if(!other_i){
            throw 1;
        }

        return std::make_shared<Bool>(value == other_i->get_value());
    }));
}