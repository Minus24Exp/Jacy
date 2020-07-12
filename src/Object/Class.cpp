#include "Class.h"

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

obj_ptr Class::call(Interpreter & ip, const ObjList & args){
    return std::make_shared<Object>(decl_scope, this);
}