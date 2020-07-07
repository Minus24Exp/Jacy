#ifndef LOCAL_H
#define LOCAL_H

#include "object/Object.h"

enum class LocalDeclType {
	Var,
	Val
};

class Local {
public:
	Local(LocalDeclType decl_type, obj_ptr val) : decl_type(decl_type), val(val) {}
	virtual ~Local() = default;

	LocalDeclType decl_type;
	obj_ptr val;
};

#endif