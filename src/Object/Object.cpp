#include "object/Object.h"
#include "object/Class.h"
#include "object/BaseFunc.h"

Object::Object() {}

Object::Object(scope_ptr scope, Class * _class)
    : Scope(scope, _class->get_instance_fields()),
      _class(_class) {}

bool Object::truthy() const {
    return true;
}

std::string Object::repr() const {
    return "<object_"+ _class->get_name() +">";
}

obj_ptr Object::get(const std::string & name) const {
    // get in object must auto-bind functions
    if(!has(name)){
        return nullptr;
    }

    obj_ptr field = Scope::get(name);
    base_func_ptr maybe_func = std::dynamic_pointer_cast<BaseFunc>(field);

    if(maybe_func){
        return maybe_func->bind(std::const_pointer_cast<Object>(shared_from_this()));
    }else{
        return field;
    }
}