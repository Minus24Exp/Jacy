#include "object/NativeFunc.h"
#include "backend/Interpreter.h"

NativeFunc::NativeFunc(scope_ptr closure,
                       const std::string & name,
                       const ParamList & params,
                       const NFBody & body)
                    : BaseFunc(closure, name, params),
                      body(body) {}

// Object //
std::string NativeFunc::repr() const {
    return "<NativeFunc:"+ name +">";
}

// BaseFunc //
obj_ptr NativeFunc::call(const ObjList & args){
    update_recursion_depth();
    
    NFArgs func_args;
    for(size_t i = 0; i < params.size(); i++){
        func_args.emplace(params[i].name, std::move(args[i]));
    }

    obj_ptr ret_val = body(std::move(func_args));

    if(!ret_val){
        // Note: This is just a helper for built-in functions
        // They can return nullptr, and then here it will be converted to Null.
        // But, nullptr does not equal to Null
        ret_val = null_obj;
    }

    return ret_val;
}

func_ptr NativeFunc::bind(obj_ptr instance){
    scope_ptr func_scope = std::make_shared<Scope>(closure);
    func_scope->define("this", {LocalDeclType::Val, instance});
    func_scope->define("[virtual_this]", {LocalDeclType::Val, instance});
    return std::make_shared<NativeFunc>(func_scope, name, params, body);
}