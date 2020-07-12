#include "object/String.h"
#include "object/Int.h"

String::String(const std::string & s) : value(s) {
    define_nf("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        // The most useless thing ever? maybe...
        // IMPORTANT: Returns new String, not self
        return std::make_shared<String>(value);
    }));

    define_nf("__add", make_nf(nullptr, "__add", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<String> other_s = cast_to_s(args["other"]);

        if(!other_s){
            throw 1;
        }

        return std::make_shared<String>(value + other_s->get_value());
    }));

    define_nf("__mul", make_nf(nullptr, "__mul", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<Int> other_i = cast_to_i(args["other"]);

        if(!other_i){
            throw 1;
        }

        std::string mul_str;
        for(int i = 0; i < other_i->get_value(); i++){
            mul_str += value;
        }

        return std::make_shared<String>(mul_str);
    }));
}