#ifndef INT_H
#define INT_H

#include "Object/Object.h"
#include "Object/String.h"
#include <string>

class Int : public Object {
public:
	Int(const int & i) : value(i) {}
	virtual ~Int() = default;

	int get_value() const {
		return value;
	}

private:
	int value;
};

#endif