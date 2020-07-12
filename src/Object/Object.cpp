#include "object/Object.h"
#include "object/Class.h"

Object::Object() {}

Object::Object(scope_ptr scope, Class * _class)
    : Scope(scope, _class->get_instance_fields())
{
    this->_class = std::shared_ptr<Class>(_class);
}

bool Object::truthy() const {
    return true;
}

std::string Object::repr() const {
    return "<object_"+ _class->get_name() +">";
}