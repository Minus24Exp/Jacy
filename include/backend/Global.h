#ifndef GLOBAL_H
#define GLOBAL_H

class Interpreter;

class Global {
public:
    Global();
    ~Global() = default;

    void reg();

private:
    Interpreter & ip;
};

#endif