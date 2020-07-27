#ifndef DICT_H
#define DICT_H

#include "object/Object.h"
#include <map>

/**
 * Dict
 */

class Dict;
using dict_ptr = std::shared_ptr<Dict>;

extern std::shared_ptr<Class> cDict;

struct DictValue {
    obj_ptr key_obj;
    obj_ptr val_obj;
};

class Dict : public Object {
public:
    Dict();
    virtual ~Dict() = default;

    // Object //
    std::string repr() const {
        return "<Dict>";
    }

    // Dict //
    obj_ptr get_item(obj_ptr key) const;
    void set_item(obj_ptr key, obj_ptr val);

    yo_int size() const;

private:
    std::map<yo_int, DictValue> elements;
};

#endif