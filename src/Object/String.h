#ifndef STRING_H
#define STRING_H

#include "Object/Object.h"

class String : public Object {
public:
	String(const std::string & v) : value(v) {}
	virtual ~String() = default;

	std::string get_value() const {
		return value;
	}

private:
	std::string value;
};

#endif