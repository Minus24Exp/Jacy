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

obj_ptr Yo_print(NFArgs && args){
    std::cout << obj_to_str(args["o"])->get_value();
    return nullptr;
}

void Global::reg(){
    const auto scope = ip.get_scope();

    // IO //
    scope->define_nf("print", make_nf(scope, "print", { {"o"} }, Yo_print));


    // Easter egg
    scope->define_nf("YOCTO", make_nf(scope, "YOCTO", {}, YOCTO));
}