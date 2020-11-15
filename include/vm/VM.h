#ifndef VM_H
#define VM_H

#include "vm/BaseVM.h"
#include "common/Logger.h"

class VM : public BaseVM {
public:
    VM();
    ~VM() override = default;

    void eval(const Chunk & chunk) override;

private:
    // Errors //
    static void error(const std::string & msg);

    // DEBUG //
public:
    VMOptions options;
    Logger log;
};

#endif // VM_H
