#ifndef MODULE_H
#define MODULE_H

#include "object/Object.h"

/**
 * Module is just a collection of fields
 *
 * As far as object stores fields, I don't have to create fields API for Module
 */

extern std::shared_ptr<Class> cModule;

class Module : public Object {
public:
    Module();
    virtual ~Module() = default;
};

#endif