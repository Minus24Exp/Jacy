#ifndef FLOAT_H
#define FLOAT_H

class Float : public Object {
public:
    Float(double value) : value {}
    virtual ~Float() = default;

private:
    double value;
};

#endif
