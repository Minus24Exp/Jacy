#include "backend/Global.h"
#include "backend/Interpreter.h"

obj_ptr io_print(NFArgs && args){
	std::cout << args["o"]->to_string() << std::endl;
	return nullptr;
}

void Global::reg(){
	const auto scope = ip.get_scope();

	// IO //
	auto print = make_nf(scope, "print", { {"o"} }, io_print);
	scope->define("print", std::move(print));
}