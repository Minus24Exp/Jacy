#include "object/Class.h"
#include "object/String.h"
#include "object/Null.h"

Class::Class(const std::string & name, class_ptr super)
    : Object(ObjectType::Class, cClass), name(name), super(super)
{
    if(super){
        define_builtin("super", super);
    }else{
        define_builtin("super", null_obj);
    }
    define_builtin("name", std::make_shared<String>(name));
}

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
    class_ptr this_class_ptr = std::static_pointer_cast<Class>(shared_from_this());
    // As far as enable_shared_from_this is template I need to cast this to class
    // Maybe it's bad architecture... aaa
    obj_ptr instance = std::make_shared<Object>(ObjectType::Object, this_class_ptr);

    // I cannot use predefined fields since class variables may be change
    instance->set_fields(get_instance_fields());

    // TODO: Important! Add magic things checkers at class declaration

    return instance;
}

void Class::set_instance_fields(const LocalMap & instance_fields){
    for(const auto & ifield : instance_fields){
        define_instance_field(ifield.first, ifield.second);
    }
}

bool Class::has_instance_field(const std::string & name) const {
    return instance_fields.find(name) != instance_fields.end();
}

void Class::define_instance_field(const std::string & name, const Local & local){
    if(has_instance_field(name)){
        throw YoctoException("Redefinition of "+ name);
    }
    instance_fields.emplace(name, local);
}

LocalMap Class::get_instance_fields() const {
    LocalMap new_instance_fields = instance_fields;
    if(super){
        // Merge with super fields
        new_instance_fields.merge(super->get_instance_fields());
    }
    return new_instance_fields;
}