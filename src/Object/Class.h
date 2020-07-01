#ifndef CLASS_H
#define CLASS_H

#include "Object/Module.h"

class Class : public Module {
public:
	Class();
	virtual ~Class() = default;

	Object * make_instance();

private:
	std::string name;
};

#endif