#ifndef FLOAT_H
#define FLOAT_H

#include "Object/Object.h"
#include <string>

class Float : public Object {
public:
	Float(const double & d) : value(d) {}
	virtual ~Float() = default;

	double get_value() const {
		return value;
	}

private:
	double value;
};

#endif