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
	std::cout << args["o"]->to_string() << std::endl;
	return nullptr;
}

void Global::reg(){
	const auto scope = ip.get_scope();

	// Primitives //
	scope->define("NullClass", {LocalDeclType::Val, NullClass});


	// IO //
	scope->define_nf("print", make_nf(scope, "print", { {"o"} }, io_print));


	// Easter egg
	scope->define_nf("YOCTO", make_nf(scope, "YOCTO", {}, YOCTO));
}