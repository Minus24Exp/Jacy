#ifndef CLASS_H
#define CLASS_H

#include "Object/Callable.h"
#include <map>

using ClassFields = std::map<std::string, Object*>;

class Class : public Callable {
public:
	Class(const std::string & name, Class * super, const ClassFields & fields);
	virtual ~Class() = default;

	Object * call(Interpreter & interpreter, const Args & args) override {
		Instance * instance = new Instance(this);

		// Find constructor
		Callable * constructor = find_field(name);
	}

	Object * find_field(const std::string & name) const;

private:
	Class * super;
	std::string name;
	ClassFields fields;
};

#endif