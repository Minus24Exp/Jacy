#ifndef SCOPE_H
#define SCOPE_H

#include "Exception.h"
#include <memory>
#include <unordered_map>

class Scope;
using scope_ptr = std::shared_ptr<Scope>;

class Object;
using obj_ptr = std::unique_ptr<Object>;

class Scope {
public:
	Scope() : parent(nullptr) {}
	Scope(scope_ptr parent) : parent(parent) {}

	scope_ptr get_parent() const {
		return parent;
	}

	void set_parent(scope_ptr parent){
		this->parent = parent;
	}

	void define(const std::string & name, obj_ptr value){
		values[name] = std::move(value);
	}

	void assign(const std::string & name, obj_ptr value){
		auto it = values.find(name);
		
		if(it != values.end()){
			values[name] = std::move(value);
		}else if(parent){
			parent->assign(name, std::move(value));
		}else{
			throw YoctoException(name + " is not defined");
		}
	}

	Object * get(const std::string & name) const {
		auto it = values.find(name);
		
		if(it != values.end()){
			return it->second.get();
		}

		if(parent){
			return parent->get(name);
		}

		return nullptr;
	}

private:
	std::unordered_map<std::string, obj_ptr> values;
	scope_ptr parent;
};

#endif