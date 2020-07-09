#ifndef SCOPE_H
#define SCOPE_H

#include "Exception.h"
#include <memory>
#include <unordered_map>
#include <iostream>
#include "backend/Local.h"

class Scope;
using scope_ptr = std::shared_ptr<Scope>;

using LocalMap = std::unordered_map<std::string, Local>;

class Scope {
public:
	Scope() : parent(nullptr) {}
	Scope(scope_ptr parent) : parent(parent) {}
	Scope(scope_ptr parent, const LocalMap & locals) : parent(parent), locals(locals) {}

	scope_ptr get_parent() const {
		return parent;
	}

	void set_parent(scope_ptr parent){
		this->parent = parent;
	}

	bool has(const std::string & name) const {
		return locals.find(name) != locals.end();
	}

	// Returns true if variable is not defined, false otherwise
	bool define(const std::string & name, const Local & loc){
		if(!has(name)){
			locals.emplace(name, loc);
			return true;
		}else{
			return false;
		}
	}

	// Returns:
	// 1 if variable was defined and able to assign
	// 0 if variable was not defined
	// -1 if variable cannot be reassigned
	int assign(const std::string & name, obj_ptr val){
		auto it = locals.find(name);
		
		if(it != locals.end()){
			if(it->second.decl_type == LocalDeclType::Val && it->second.val != nullptr){
				return -1;
			}
			locals.at(name).val = val;
			return 1;
		}else if(parent){
			return parent->assign(name, val);
		}else{
			return 0;
		}
	}

	obj_ptr get(const std::string & name) const {
		auto it = locals.find(name);
		
		if(it != locals.end()){
			return it->second.val;
		}

		if(parent){
			return parent->get(name);
		}

		return nullptr;
	}

	LocalMap get_locals() const {
		return locals;
	}

	// Helpers //
	void define_nf(const std::string & name, const obj_ptr & nf){
		if(!define(name, {LocalDeclType::Val, nf})){
			throw YoctoException("Attempt to redefine native function "+ name);
		}
	}

private:
	LocalMap locals;
	scope_ptr parent;
};

#endif