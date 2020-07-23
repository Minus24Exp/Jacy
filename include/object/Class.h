#ifndef CLASS_H
#define CLASS_H

#include "object/Callable.h"

class Class;
using class_ptr = std::shared_ptr<Class>;

const auto cast_to_class = [](obj_ptr obj) -> class_ptr { return std::dynamic_pointer_cast<Class>(obj); };

extern std::shared_ptr<Class> cClass;

class Class : public Object, public Callable {
public:
    Class(const std::string & name, class_ptr super);
    virtual ~Class() = default;

    // Object //
    std::string repr() const override;

    // Callable //
    size_t required_argc() const override;
    size_t argc() const override;

    obj_ptr call(const ObjList & args) override;

    // Class //
    std::string get_name() const {
        return name;
    }

    class_ptr get_super() const {
        return super;
    }

private:
    std::string name;
    class_ptr super;

    LocalMap get_instance_fields() const;
};

#endif