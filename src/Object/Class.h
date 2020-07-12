#ifndef CLASS_H
#define CLASS_H

#include "object/Callable.h"

/**
 * Class - Some kind of factory for Objects
 *
 */

class Class;
using class_ptr = std::shared_ptr<Class>;

class Class : public Object, public Callable {
public:
    Class(scope_ptr decl_scope, const std::string & name, class_ptr super);
    virtual ~Class() = default;
       
    // Object //
    std::string repr() const override;

    // Callable //
    size_t required_argc() const override;
    size_t argc() const override;

    obj_ptr call(Interpreter & ip, const ObjList & args) override;

    // Class //
    std::string get_name() const {
        return name;
    }

    LocalMap get_instance_fields() const {
        return decl_scope->get_locals();
    }

private:
    scope_ptr decl_scope;
    std::string name;
    class_ptr super;
};

#endif