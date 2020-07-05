#include "backend/Global.h"
#include "backend/Interpreter.h"

const Params io_print_params { std::move(Param("o")) };
obj_ptr io_print(NFArgs && args){
	std::cout << args["o"]->to_string() << std::endl;
	return nullptr;
}

void Global::reg(){
	const auto scope = ip.get_scope();

	// IO //
	auto print = make_nf(scope, "print", io_print_params);
	scope->define("print", std::move(print));
}