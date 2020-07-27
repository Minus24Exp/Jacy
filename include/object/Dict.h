#ifndef DICT_H
#define DICT_H

#include "object/Object.h"
#include <unordered_map>

extern std::shared_ptr<Class> cDict;

using DictElements = std::unordered_map<yo_int, obj_ptr>;

class Dict : public Object {
public:
    Dict();
    virtual ~Dict() = default;

    void set_elements(const DictElements & elements);
    DictElements get_elements() const;

    obj_ptr get_item(obj_ptr key) const;
    void set_item(obj_ptr key, obj_ptr val);

    yo_int size() const;

private:
    DictElements elements;
};

#endif