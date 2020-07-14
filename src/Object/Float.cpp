#include "object/Float.h"
#include "object/String.h"

Float::Float(double d) : value(d) {
    define_nf("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return std::make_shared<String>(std::to_string(value));
    }));
}