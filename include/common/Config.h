#ifndef CONFIG_H
#define CONFIG_H

#include "common/Options.h"
#include "Exception.h"

#include <string>
#include <vector>

namespace jc::common {
    class Config {
    public:
        Config() = default;
        virtual ~Config() = default;

        void config(int argc, const char ** argv);

        JacyOptions jacy_options;
        LexerOptions lexer_options;
        ParserOptions parser_options;
        CompilerOptions compiler_options;
        DisasmOptions disasm_options;
        VMOptions vm_options;
    };
}

#endif // CONFIG_H
