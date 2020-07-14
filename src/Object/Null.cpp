#include "object/Null.h"
#include "object/String.h"

Null::Null() {
    define_nf("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        return std::make_shared<String>("Null");
    }));
}