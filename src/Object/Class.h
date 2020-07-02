#ifndef CLASS_H
#define CLASS_H

#include "Object/Object.h"
#include "Object/NativeMethod.h"

/**
 * Classes is kinda factory for objects
 */

class Class : public Object {
public:
	Class(const ObjectFields & fields);
	virtual ~Class() = default;

	void extend(Class * super);

	Class * get_super() const {
		return super;
	}

private:
	Class * super;
	ObjectFields fields;

	std::string name;
};

#endif