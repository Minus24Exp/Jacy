#include "object/Bool.h"
#include "object/String.h"

Bool::Bool(bool b) : value(b) {
    define_nf("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return std::make_shared<String>(value ? "true" : "false");
    }));

    define_nf("__not", make_nf(nullptr, "__not", {}, [this](NFArgs && args){
        return std::make_shared<Bool>(!value);
    }));
}