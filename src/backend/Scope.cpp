#include "backend/Scope.h"
#include "object/Object.h"
#include "object/BaseFunc.h"

bool Scope::has(const std::string & name) const {
    return locals.find(name) != locals.end();
}

bool Scope::define(const std::string & name, const Local & loc){
    if(!has(name)){
        locals.emplace(name, loc);
        return true;
    }else{
        return false;
    }
}

int Scope::assign(const std::string & name, obj_ptr val){
    auto it = locals.find(name);

    if(it != locals.end()){
        obj_ptr local_val = locals.at(name).val;

        if(local_val != nullptr && local_val->get_obj_type() == ObjectType::Func){
            func_ptr maybe_setter = cast_to_func(local_val);
            if(maybe_setter->get_mode() == FuncMode::Set){
                maybe_setter->call({val});
                return 1;
            }
        }

        // For `val` nullptr means that it wasn't assigned yet
        if(it->second.decl_type == LocalDeclType::Val && local_val != nullptr){
            return -1;
        }

        locals.at(name).val = val;
        return 1;
    }else if(has("[virtual_this]")){
        // Virtual-this
        return get("[virtual_this]")->set(name, val);
    }else if(parent){
        return parent->assign(name, val);
    }else{
        return 0;
    }
}

obj_ptr Scope::get(const std::string & name) const {
    if(has(name)){
        return locals.at(name).val;
    }

    // Virtual-this
    if(has("[virtual_this]")){
        obj_ptr virtual_this = get("[virtual_this]");
        if(virtual_this->has(name)){
            return virtual_this->get(name);
        }
    }

    if(parent){
        return parent->get(name);
    }

    return nullptr;
}

// Helpers //
void Scope::define_nf(const std::string & name, obj_ptr nf){
    if(!define(name, {LocalDeclType::Val, nf})){
        throw YoctoException("Attempt to redefine native function "+ name);
    }
}

// DEBUG //
void Scope::print() const {
    std::cout << "(";
    for(const auto & l : locals){
        std::cout << (l.second.decl_type == LocalDeclType::Val ? "val" : "var") << " " << l.first
                  << " " << (l.second.val ? l.second.val->repr() : "null") << "; ";
    }
    std::cout << ")";
}

void Scope::print_with_lookup() const {
    print();
    if(parent){
        std::cout << "[";
        parent->print_with_lookup();
        std::cout << "]";
    }
    std::cout << std::endl;
}