#ifndef ARRAY_H
#define ARRAY_H

#include "object/Object.h"

class Array;
using array_ptr = std::shared_ptr<Array>;

const auto cast_to_a = [](obj_ptr obj) -> array_ptr { return std::dynamic_pointer_cast<Array>(obj); };

class Array : public Object {
public:
	Array();
    Array(const ObjList & elements);
	virtual ~Array() = default;
	
    // Object //
    bool truthy() const override;
    std::string repr() const override;

    // Array //
    
    // Functions for built-in behaviour
    ObjList get_elements() const;
    void set_elements(const ObjList & elements);
    yo_int size() const;

    // Function for API
    obj_ptr get_item(yo_int index) const;

    // return true if index not out of bound
    bool set_item(yo_int index, obj_ptr value);

    void append(obj_ptr el);

    // Helpers //
    yo_int abs_index(yo_int index) const;

private:
    ObjList elements;
};

#endif