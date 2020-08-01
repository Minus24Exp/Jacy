#include "object/Dict.h"
#include "object/NativeFunc.h"
#include "object/String.h"

Dict::Dict() : Object(ObjectType::Dict, cDict)
{
    define_builtin("__getitem", make_nf(nullptr, "__getitem", { {"key"} }, [this](NFArgs && args){
        return get_item(args["key"]);
    }));

    define_builtin("__setitem", make_nf(nullptr, "__setitem", { {"key"}, {"val"} }, [this](NFArgs && args){
        set_item(args["key"], args["val"]);
        return nullptr;
    }));

    define_builtin("to_s", make_nf(nullptr, "to_s", {}, [this](NFArgs && args){
        std::string str = "{";

        for(auto it = elements.begin(); it != elements.end(); it++){
            str += obj_to_str(it->second.key_obj);
            str += ": ";
            str += obj_to_str(it->second.val_obj);

            if(it != std::prev(elements.end())){
                str += ", ";
            }
        }
        str += "}";

        return make_string(str);
    }));
}

obj_ptr Dict::get_item(obj_ptr key) const {
    yo_int hash = obj_hash(key);

    if(elements.find(hash) == elements.end()){
        throw YoctoException("Key `"+ obj_to_str(key) +"` is not found");
    }

    return elements.at(hash).val_obj;
}

void Dict::set_item(obj_ptr key, obj_ptr val){
    yo_int hash = obj_hash(key);
    elements[hash] = {key, val};
}

yo_int Dict::size() const {
    return elements.size();
}