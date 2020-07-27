#include "object/Dict.h"
#include "object/NativeFunc.h"

Dict::Dict() : Object(ObjectType::Dict, cDict)
{
    define_builtin("__getitem", make_nf(nullptr, "__getitem", { {"key"} }, [this](NFArgs && args){
        return get_item(args["key"]);
    }));
}

obj_ptr Dict::get_item(obj_ptr key) const {
    yo_int hash = obj_hash(key);

    if(elements.find(hash) == elements.end()){
        throw YoctoException("Key `"+ obj_to_str(key) +"` is not found");
    }

    return elements.at(hash);
}

void Dict::set_item(obj_ptr key, obj_ptr val){
    yo_int hash = obj_hash(key);
    elements[hash] = val;
}

yo_int Dict::size() const {
    return elements.size();
}