#include "backend/Global.h"
#include "backend/Interpreter.h"

void Global::reg(){
	const auto scope = ip.get_scope();

	// IO //
	auto print = make_nf(scope, "print", { {"o"} }, [this](NFArgs && args) -> obj_ptr {
		std::cout << args["o"]->to_string() << std::endl;
		return nullptr;
	});

	scope->define("print", std::move(print));
}