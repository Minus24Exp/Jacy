#ifndef CLASS_H
#define CLASS_H

#include "Object/Object.h"

class Class : public Object {
public:
	Class();
	virtual ~Class() = default;

	Object * make_instance(const ObjectFields & fields);

private:
	std::string name;
};

#endif