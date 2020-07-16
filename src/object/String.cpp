#include "object/String.h"
#include "object/Int.h"

#include <sstream>

String::String(const std::string & s) : value(s) {
    define_nf("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        // The most useless thing ever? maybe...
        // IMPORTANT: Returns new String, not self
        return std::make_shared<String>(value);
    }));

    define_nf("__add", make_nf(nullptr, "__add", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<String> other_s = cast_to_s(args["other"]);

        if(!other_s){
            throw YoctoException("Invalid right-hand type in string `+` operator, string expected");
        }

        return std::make_shared<String>(value + other_s->get_value());
    }));

    define_nf("__mul", make_nf(nullptr, "__mul", { {"other"} }, [this](NFArgs && args){
        std::shared_ptr<Int> other_i = cast_to_i(args["other"]);

        if(!other_i){
            throw YoctoException("Invalid right-hand type in string `*` operator, int expected");
        }

        auto N = other_i->get_value();

        if(N < 0){
            N = 0;
        }

        if(N == 0){
            return std::make_shared<String>("");
        }

        // I've optimized it as much as I can...

        if(value.size() > 1){
            const auto len = value.size();
            std::string mul_str;
            mul_str.reserve(N * len + 1);
            mul_str += value;

            size_t m = 2;
            for(; m < N; m *= 2){
                mul_str += mul_str;
            }
            mul_str.append(mul_str.c_str(), (N - (m / 2)) * len);

            return std::make_shared<String>(mul_str);

        }else if(value.size() == 1){
            return std::make_shared<String>(std::string(N, value.front()));
        }else{
            return std::make_shared<String>(value);
        }
    }));
}