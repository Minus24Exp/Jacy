#ifndef LIST_H
#define LIST_H

#include "object/Object.h"

class List;
using list_ptr = std::shared_ptr<List>;

extern std::shared_ptr<Class> cList;

class List : public Object {
public:
    List();
    virtual ~List() = default;

    // Class //
    bool truthy() const override;
    std::string repr() const override;

    // List //
    ObjList get_elements() const;
    void set_elements(const ObjList & elements);
    yo_int size() const;

    obj_ptr get_item(yo_int index) const;

    bool set_item(yo_int index, obj_ptr value);

    void append(obj_ptr el);

    // Normalize index (-1 == size - 1)
    yo_int norm_index(yo_int index) const;

private:
    ObjList elements;
};

#endif