#ifndef SCOPE_H
#define SCOPE_H

#include "Exception.h"

#include <unordered_map>
#include <iostream>

struct NativeMethod;

/**
 * TODO:
 * - Add mutliple definitions of function for overloading when types will be implemented
 */

class Scope {
public:
	Scope(Scope * parent = nullptr);
	virtual ~Scope() = default;

	Scope * get_parent() const {
		return parent;
	}

	void set_parent(Scope * parent){
		this->parent = parent;
	}

	bool has(const std::string & name) const;
	Object * get(const std::string & name) const;
	Object * lookup(const std::string & name) const;
	void set(const std::string & name, Object * val);
	void define(const std::string & name, Object * val);

private:
	Scope * parent;

	std::unordered_map<std::string, Object*> locals;
};

#endif