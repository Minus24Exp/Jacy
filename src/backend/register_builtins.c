#ifndef REGISTER_BUILTINS_H
#define REGISTER_BUILTINS_H

#include "backend/Scope.h"
#include "Object/NativeMethod.h"
#include "Object/NullClass.h"
#include "Object/String.h"
#include "BaseVisitor.h"

inline register_builtins(BaseVisitor & visitor){
	->define("print", make_native_method({ "o" }, [&](const NativeMethodArgs & args) -> Object* {
		if(!args.at("o")->has_method("to_s")){
			throw YoctoException("Unable to convert object to string");
		}
		String * str = dynamic_cast<String*>(args.at("o")->find_method("to_s")->call(*g, {}));
		std::cout << str->get_value() << std::endl;

		return new NullClass;
	}));
}

#endif