#ifndef CLASS_H
#define CLASS_H

#include "object/Callable.h"

/**
 * Class
 *
 * instance_fields - LocalMap for fields that will be defined in every instance of class
 * To define field for instance use methods with `instance_field` suffix.
 * To define field for class (as Object) use Object's `set`.
 */

class Class;
using class_ptr = std::shared_ptr<Class>;

const auto cast_to_class = [](obj_ptr obj) { return std::dynamic_pointer_cast<Class>(obj); };
const auto s_cast_to_class = [](obj_ptr obj) { return std::static_pointer_cast<Class>(obj); };

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

    bool has_instance_field(const std::string & name) const;
    void set_instance_fields(const LocalMap & instance_fields);
    void define_instance_field(const std::string & name, const Local & local);

private:
    LocalMap instance_fields;
    std::string name;
    class_ptr super;

    LocalMap get_instance_fields() const;
};

#endif