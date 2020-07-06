#include "backend/Global.h"
#include "backend/Interpreter.h"

obj_ptr io_print(NFArgs && args){
	std::cout << args["o"]->to_string() << std::endl;
	return nullptr;
}

void Global::reg(){
	const auto scope = ip.get_scope();

	// IO //
	Params io_print_params;
	Param io_print_p1("o");
	io_print_params.push_back(std::move(io_print_p1));
	auto print = make_nf(scope, "print", std::move(io_print_params), io_print);
	scope->define("print", std::move(print));
}