#ifndef GLOBAL_H
#define GLOBAL_H

#include "object/objects.h"

class Interpreter;

/**
 * Global is used to register globals
 */

class Global {
public:
    Global(Interpreter & ip) : ip(ip) {}
    virtual ~Global() = default;

    void reg();

private:
    Interpreter & ip;
};

#endif