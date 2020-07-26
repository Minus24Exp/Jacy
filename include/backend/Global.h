#ifndef GLOBAL_H
#define GLOBAL_H

#include <memory>

class Scope;
class Interpreter;

class Global {
public:
    Global();
    ~Global() = default;

    void reg();

private:
    Interpreter & ip;
    std::shared_ptr<Scope> g_scope;

    // Just split classes registration
    void reg_object();
    void reg_class();
    void reg_null();
    void reg_bool();
    void reg_int();
    void reg_float();
    void reg_string();
    void reg_func();
    void reg_module();
    void reg_list();
};

#endif