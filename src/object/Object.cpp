#include "object/Object.h"
#include "object/Class.h"
#include "object/BaseFunc.h"
#include "object/String.h"
#include "object/Class.h"
#include "object/Null.h"

Object::Object(){
    if(_class){
        define("class", {LocalDeclType::Val, std::shared_ptr<Class>(_class)});
    }else{
        define("class", {LocalDeclType::Val, null_obj});
    }
}

Object::Object(scope_ptr scope, class_ptr _class)
    : Scope(scope, _class->get_instance_fields()),
      _class(_class) {}

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

obj_ptr Object::get(const std::string & name) const {
    obj_ptr field = Scope::get(name);

    if(!field){
        return nullptr;
    }

    func_ptr maybe_func = std::dynamic_pointer_cast<BaseFunc>(field);

    if(maybe_func){
        return maybe_func->bind(std::const_pointer_cast<Object>(shared_from_this()));
    }else{
        return field;
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