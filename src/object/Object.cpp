#include "object/Object.h"
#include "object/Class.h"
#include "object/BaseFunc.h"
#include "object/String.h"
#include "object/Class.h"
#include "object/Null.h"
#include "object/Int.h"
#include "object/NativeFunc.h"
#include "object/Bool.h"

std::string obj_to_str(obj_ptr obj){
    if(!obj){
        return "null";
    }

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

yo_int obj_hash(obj_ptr obj){
    // null != null_ptr, so if obj is nullptr then there's a problem in source
    if(!obj){
        throw DevError("Passing null to obj_hash");
    }

    if(!obj->has("__hash")){
        throw YoctoException("Invalid key (key must have __hash function)");
    }

    func_ptr hash_func = cast_to_func(obj->get("__hash"));

    if(hash_func){
        int_ptr int_obj = cast_to_i(hash_func->call());
        if(int_obj){
            return int_obj->get_value();
        }
    }

    throw YoctoException("Invalid __hash function (must return integer)");
}

// Try to call object's `__eq` function or return nullptr if it is not valid
bool_ptr obj_eq(obj_ptr obj1, obj_ptr obj2){
    // Check if element has `__eq` function that returns boolean
    if(!obj1->has("__eq")){
        return nullptr;
    }

    obj_ptr __eq = obj1->get("__eq");

    if(__eq->get_obj_type() != ObjectType::Func){
        return nullptr;
    }

    obj_ptr __eq_result = std::static_pointer_cast<BaseFunc>(obj1->get("__eq"))->call({obj2});

    if(__eq_result->get_obj_type() != ObjectType::Bool){
        return nullptr;
    }

    return std::static_pointer_cast<Bool>(__eq_result);
}

Object::Object(ObjectType obj_type, class_ptr _class) : obj_type(obj_type), _class(_class) {
    define_m_builtin("__class", _class);
}

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

void Object::set_fields(const LocalMap & instance_fields){
    // `set_fields` is called after all built-ins are registered
    // so I need to check if some fields are built-ins
    
    // This is the place where all fields modifiers will be implemented

    for(const auto i_field : instance_fields){
        const auto name = i_field.first;
        const auto redef_it = fields.find(name);

        if(redef_it != fields.end()){
            // Check for built-in redefinition
            if(redef_it->second.decl_type == LocalDeclType::Builtin){
                throw YoctoException("Redefinition of built-in "+ name);
            }
        }

        fields.emplace(name, Local(i_field.second.decl_type, i_field.second.val));
    }
}

bool Object::has(const std::string & name) const {
    return fields.find(name) != fields.end();
}

obj_ptr Object::get(const std::string & name) const {
    if(has(name)){
        return fields.at(name).val;
    }

    return nullptr;
}

int Object::set(const std::string & name, obj_ptr value){
    auto it = fields.find(name);

    if(it != fields.end()){
        if(it->second.decl_type == LocalDeclType::Val && it->second.val != nullptr){
            return -1;
        }
        fields.at(name).val = value;
        return 1;
    }else{
        return 0;
    }
}

void Object::define_builtin(const std::string & name, obj_ptr value){
    if(has(name) && fields.at(name).decl_type != LocalDeclType::MutBuiltin){
        throw DevError("Redefinition of built-in "+ name);
    }
    fields.emplace(name, Local(LocalDeclType::Builtin, value));
}

void Object::define_m_builtin(const std::string & name, obj_ptr value){
    if(has(name) && fields.at(name).decl_type != LocalDeclType::MutBuiltin){
        throw DevError("Redefinition of built-in "+ name);
    }
    fields.emplace(name, Local(LocalDeclType::MutBuiltin, value));
}