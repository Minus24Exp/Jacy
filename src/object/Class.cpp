#include "object/Class.h"
#include <iostream>

Class::Class(const std::string & name, class_ptr super)
    : Object(ObjectType::Class, cClass), name(name), super(super) {}

std::string Class::repr() const {
    return "<class:"+ name +">";
}

size_t Class::required_argc() const {
    return 0;
}

size_t Class::argc() const {
    return 0;
}

obj_ptr Class::call(const ObjList & args){
    // As far as enable_shared_from_this is template I need to cast this to class
    // Maybe it's bad architecture... aaa
    obj_ptr instance = std::make_shared<Object>(ObjectType::Object, std::static_pointer_cast<Class>(shared_from_this()));
    
    // I cannot use predefined fields since class variables may be change
    instance->set_instance_fields(get_instance_fields());
    return instance;
}

void Class::set_fields(const LocalMap & fields){
    this->fields = fields;
}

LocalMap Class::get_instance_fields() const {
    LocalMap instance_fields = fields;
    if(super){
        // Merge with super fields
        instance_fields.merge(super->get_instance_fields());
    }
    return instance_fields;
}