#include "object/Object.h"
#include "object/Class.h"
#include "object/BaseFunc.h"
#include "object/String.h"
#include "object/Class.h"
#include "object/Null.h"

Object::Object(ObjectType obj_type, class_ptr _class) : obj_type(obj_type), _class(_class) {}

bool Object::truthy() const {
    return true;
}

std::string Object::repr() const {
    return "<object_"+ _class->get_name() +">";
}

bool Object::is(class_ptr check_class) const {
    class_ptr check = _class;

    while(check){
        if(check == check_class){
            return true;
        }else if(check->get_super()){
            check = check->get_super();
        }else{
            return false;
        }
    }

    return false;
}

void Object::set_instance_fields(const LocalMap & fields){
    instance_fields = fields;
}

bool Object::has(const std::string & name) const {
    return instance_fields.find(name) != instance_fields.end();
}

obj_ptr Object::get(const std::string & name) const {
    if(has(name)){
        return instance_fields.at(name).val;
    }

    return nullptr;
}

int Object::set(const std::string & name, obj_ptr value){
    auto it = instance_fields.find(name);
    
    if(it != instance_fields.end()){
        if(it->second.decl_type == LocalDeclType::Val && it->second.val != nullptr){
            return -1;
        }
        instance_fields.at(name).val = value;
        return 1;
    }else{
        return 0;
    }
}

std::string obj_to_str(obj_ptr obj){
    // If object has method `to_s` and it returns string then use it
    if(obj->has("to_s")){
        func_ptr to_s = cast_to_func(obj->get("to_s"));
        if(to_s){
            string_ptr string = cast_to_s(to_s->call());
            if(string){
                return string->get_value();
            }
        }
    }

    // Otherwise represent object
    return obj->repr();
}