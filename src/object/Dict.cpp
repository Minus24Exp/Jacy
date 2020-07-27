#include "object/Dict.h"

Dict::Dict() : Object(ObjectType::Dict, cDict) {}

void Dict::set_elements(const DictElements & elements){
    this->elements = elements;
}

DictElements Dict::get_elements() const {
    return elements;
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