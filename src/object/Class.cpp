#include "object/Class.h"

Class::Class(scope_ptr decl_scope, const std::string & name, class_ptr super)
            : decl_scope(decl_scope), name(name), super(super) {}

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
    return std::make_shared<Object>(decl_scope, shared_from_this());
}

LocalMap Class::get_instance_fields() const {
    LocalMap fields = decl_scope->get_locals();
    if(super){
        // Merge with super fields
        fields.merge(super->get_instance_fields());
    }
    return fields;
}