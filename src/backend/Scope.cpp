#include "backend/Scope.h"

Scope::Scope(Scope * parent){
	set_parent(parent);
}

bool Scope::has(const std::string & name) const {
	return locals.find(name) != locals.end();
}

// Note: Use `get` only with `has`
Object * Scope::get(const std::string & name) const {
	if(has(name)){
		return locals.at(name);
	}else{
		// TODO: Add error position
		throw YoctoException(name + " is not defined");
	}
}

Object * Scope::lookup(const std::string & name) const {
	if(has(name)){
		return get(name);
	}else if(parent){
		return parent->lookup(name);
	}else{
		throw YoctoException(name + " is not defined");
	}
}

void Scope::set(const std::string & name, Object * val){
	if(has(name)){
		locals[name] = val;
	}else if(parent){
		parent->set(name, val);
	}else{
		throw YoctoException(name + " is not defined");
	}
}

void Scope::define(const std::string & name, Object * val){
	if(has(name)){
		throw name + " has been already defined";
	}else{
		locals[name] = val;
	}
}