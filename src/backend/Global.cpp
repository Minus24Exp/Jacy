#include "backend/Global.h"
#include "backend/Interpreter.h"


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

obj_ptr io_print(NFArgs && args){
    // If object has method `to_s` and it returns string then use it
    if(args["o"]->has("to_s")){
        func_ptr to_s = cast_to_func(args["o"]->get("to_s"));
        if(to_s){
            string_ptr string = cast_to_s(to_s->call());
            if(string){
                std::cout << string->get_value() << std::endl;
                return nullptr;
            }
        }
    }

    // Otherwise represent object
    std::cout << args["o"]->repr() << std::endl;
    return nullptr;
}

void Global::reg(){
    const auto scope = ip.get_scope();

    // IO //
    scope->define_nf("print", make_nf(scope, "print", { {"o"} }, io_print));


    // Easter egg
    scope->define_nf("YOCTO", make_nf(scope, "YOCTO", {}, YOCTO));
}