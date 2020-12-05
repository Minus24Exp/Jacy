#ifndef CONFIG_H
#define CONFIG_H

#include "common/Options.h"
#include "Exception.h"

#include <string>
#include <vector>
#include <map>

namespace jc::common {
    struct CLArg {
        // If CLArg if `flag` no value for it expected it's just string "1" or "0"
        bool flag;
        std::string value;
    };

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

        // cl_args describes default command line arguments that may be replaced if any of them passed
        static std::map<std::string, CLArg> cl_args;
    };
}

#endif // CONFIG_H
