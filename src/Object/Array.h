#ifndef ARRAY_H
#define ARRAY_H

#include "object/Object.h"

// I use size_t because it's right type for vector
// but anyway Int contains long, so maybe Int index
// can be not enough for large arrays...

class Array;
using array_ptr = std::shared_ptr<Array>;

const auto cast_to_a = [](obj_ptr obj) -> array_ptr { return std::dynamic_pointer_cast<Array>(obj); };

class Array : public Object {
public:
	Array();
	virtual ~Array() = default;
	
    // Object //
    bool truthy() const override;
    std::string repr() const override;

    // Array //
    
    // Functions for built-in behaviour
    ObjList get_elements() const;
    void set_elements(const ObjList & elements);
    size_t size() const;

    // Function for API
    obj_ptr get_item(size_t index) const;

    // return true if index not out of bound
    bool set_item(size_t index, obj_ptr value);

    void append(obj_ptr el);

    // Helpers //
    inline size_t abs_index(size_t index) const {
        return index < 0 ? size() - 1 : index;
    }

private:
    ObjList elements;
};

#endif