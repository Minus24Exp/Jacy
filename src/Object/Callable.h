#ifndef CALLABLE_H
#define CALLABLE_H

class Interpreter;
class Object;

using Args = std::vector<Object*>;

class Callable {
public:
	virtual ~Callable(){}

	virtual Object * call(Interpreter & interpreter, const Args & args) = 0; 
};

#endif