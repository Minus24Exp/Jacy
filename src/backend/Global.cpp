#include "backend/Global.h"
#include "backend/Interpreter.h"
#include "object/objects.h"

#include <iostream>

Global::Global() : ip(Interpreter::get_instance()) {
    g_scope = ip.get_scope();
}

std::shared_ptr<Null> null_obj;

// Classes //
class_ptr cObject;
class_ptr cClass;
class_ptr cNull;
class_ptr cBool;
class_ptr cInt;
class_ptr cFloat;
class_ptr cString;
class_ptr cFunc;
class_ptr cModule;
class_ptr cList;
class_ptr cDict;
class_ptr cRange;

// Easter egg
obj_ptr YOCTO(NFArgs && args){
    std::cout <<
    "(_|   |                        \n"
    "  |   |   __    __   _|_   __  \n"
    "  |   |  /  \\  /      |   /  \\\n"
    "   \\_/|/ \\__/  \\___/  |_/ \\__/\n"
    "     /|\n"
    "     \\|\n"
    << std::endl;
    return nullptr;
}

obj_ptr Yo_print(NFArgs && args){
    std::cout << obj_to_str(args["o"]);
    return nullptr;
}

obj_ptr Yo_println(NFArgs && args){
    std::cout << obj_to_str(args["o"]) << std::endl;
    return nullptr;
}

obj_ptr Yo_repr(NFArgs && args){
    std::cout << args["o"]->repr() << std::endl;
    return nullptr;
}

void Global::reg(){    
    // Classes //
    reg_object();
    reg_class();
    reg_null();
    reg_bool();
    reg_int();
    reg_float();
    reg_string();
    reg_func();
    reg_module();
    reg_list();
    reg_dict();
    reg_range();
    
    // IO //
    g_scope->define_nf("print", make_nf(g_scope, "print", { {"o"} }, Yo_print));
    g_scope->define_nf("println", make_nf(g_scope, "println", { {"o"} }, Yo_println));
    g_scope->define_nf("repr", make_nf(g_scope, "repr", { {"o"} }, Yo_repr));

    // Easter egg
    g_scope->define_nf("YOCTO", make_nf(g_scope, "YOCTO", {}, YOCTO));
}

void Global::reg_object(){
    cObject = std::make_shared<Class>("Object", nullptr);
    g_scope->define("Object", {LocalDeclType::Val, cObject});
}

void Global::reg_class(){
    cClass = std::make_shared<Class>("Class", cObject);
    g_scope->define("Class", {LocalDeclType::Val, cClass});
}

void Global::reg_null(){
    cNull = std::make_shared<Class>("NullClass", cObject);
    g_scope->define("NullClass", {LocalDeclType::Val, cNull});

    null_obj = std::make_shared<Null>();
}

void Global::reg_bool(){
    cBool = std::make_shared<Class>("Bool", cObject);
    g_scope->define("Bool", {LocalDeclType::Val, cBool});
}

void Global::reg_int(){
    cInt = std::make_shared<Class>("Int", cObject);
    g_scope->define("Int", {LocalDeclType::Val, cInt});
}

void Global::reg_float(){
    cFloat = std::make_shared<Class>("Float", cObject);
    g_scope->define("Float", {LocalDeclType::Val, cFloat});
}

void Global::reg_string(){
    cString = std::make_shared<Class>("String", cObject);
    g_scope->define("String", {LocalDeclType::Val, cString});
}

void Global::reg_func(){
    cFunc = std::make_shared<Class>("Func", cObject);
    g_scope->define("Func", {LocalDeclType::Val, cFunc});
}

void Global::reg_module(){
    cModule = std::make_shared<Class>("Module", cModule);
    g_scope->define("Module", {LocalDeclType::Val, cModule});
}

void Global::reg_list(){
    cList = std::make_shared<Class>("List", cList);

    g_scope->define("List", {LocalDeclType::Val, cList});
}

void Global::reg_dict(){
    cDict = std::make_shared<Class>("Dict", cDict);
    g_scope->define("Dict", {LocalDeclType::Val, cDict});
}

void Global::reg_range(){
    cRange = std::make_shared<Class>("Range", cRange);
    g_scope->define("Range", {LocalDeclType::Val, cRange});
}