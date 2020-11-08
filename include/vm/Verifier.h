#ifndef VERIFIER_H
#define VERIFIER_H

#include "vm/BaseVM.h"
#include "Exception.h"

/**
 * Verifier
 * Verifies bytecode for type matching (now only that)
 */
class Verifier : public BaseVM {
public:
    Verifier();
    ~Verifier() override = default;

    void eval(const Chunk & chunk) override;
};

#endif // VERIFIER_H
