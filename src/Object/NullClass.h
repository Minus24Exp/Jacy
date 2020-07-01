#ifndef NULLCLASS_H
#define NULLCLASS_H

#include "Object/Object.h"

class NullClass : public Object {
public:
	NullClass() {}
	virtual ~NullClass() = default;
};

#endif