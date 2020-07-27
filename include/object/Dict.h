#ifndef DICT_H
#define DICT_H

#include "object/Object.h"
#include <unordered_map>

class Dict;
using dict_ptr = std::shared_ptr<Dict>;

extern std::shared_ptr<Class> cDict;

class Dict : public Object {
public:
    Dict();
    virtual ~Dict() = default;

    obj_ptr get_item(obj_ptr key) const;
    void set_item(obj_ptr key, obj_ptr val);

    yo_int size() const;

private:
    std::unordered_map<yo_int, obj_ptr> elements;
};

#endif