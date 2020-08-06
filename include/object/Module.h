#ifndef MODULE_H
#define MODULE_H

#include "object/Object.h"

/**
 * Module is just a collection of declarations
 *
 * As far as Object stores fields, I can use it for module insides
 * 
 * `as` - the name module was imported as
 * 
 */

class Module;
using module_ptr = std::shared_ptr<Module>;

extern std::shared_ptr<Class> cModule;

class Module : public Object {
public:
    Module(const LocalMap & insides);
    virtual ~Module() = default;

    std::string repr() const {
        return "<Module>";
    }
    
};

#endif