#ifndef COMMON_H
#define COMMON_H

namespace jc {
    /**
     * Just some common things
     */

    // Note: I used enum for declaration type, but not a bool
    //  because I want to add `const` in the future
    enum class VarDeclKind {Var, Val};
}

#endif // COMMON_H
