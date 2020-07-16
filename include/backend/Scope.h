#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <unordered_map>
#include <iostream>

#include "Exception.h"
#include "backend/Local.h"

class Scope;
using scope_ptr = std::shared_ptr<Scope>;

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

    virtual bool has(const std::string & name) const;

    // Returns true if variable is not defined, false otherwise
    virtual bool define(const std::string & name, const Local & loc);

    // Returns:
    // 1 if variable was defined and able to assign
    // 0 if variable was not defined
    // -1 if variable cannot be reassigned
    virtual int assign(const std::string & name, obj_ptr val);

    virtual obj_ptr get(const std::string & name) const;

    LocalMap get_locals() const {
        return locals;
    }

    void set_locals(const LocalMap & locals){
        this->locals = locals;
    }

    // Helpers //
    void define_nf(const std::string & name, obj_ptr nf);

    // DEBUG //
    void print() const;
    void print_with_lookup() const;
    
private:
    LocalMap locals;
    scope_ptr parent;
};

#endif