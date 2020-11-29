#include "common/Config.h"

namespace jc::common {
    void Config::config(int argc, const char ** argv) {
        // jacy_args -> false when main file appears
        bool jacy_args = true;

        std::vector<std::string> script_argv;
        for (int i = 1; i < argc; i++) {
            std::string arg(argv[i]);

            if (arg[0] == '-') {
                if (jacy_args) {
                    // Parse Jacy arguments
                    if (arg.substr(1) == "debug") {
                        jacy_options.debug = true;
                    }
                } else {
                    script_argv.emplace_back(argv[i]);
                }
            } else {
                // Check if argument is Jacy file (ends with ".jc")
                if (arg.size() > 3 && arg.compare(arg.size() - 3, 3, ".jc") == 0) {
                    if (jacy_options.main_file.empty()) {
                        jacy_args = false;
                        jacy_options.main_file = arg;
                    } else {
                        throw JacyException("Expected only one input file");
                    }
                }
            }
        }

        if (!jacy_options.debug) {
            jacy_options.log.level = common::LogLevel::Warn;
            lexer_options.log.level = common::LogLevel::Warn;
            parser_options.log.level = common::LogLevel::Warn;
            compiler_options.log.level = common::LogLevel::Warn;
            disasm_options.log.level = common::LogLevel::Warn;
            vm_options.log.level = common::LogLevel::Warn;
        }
    }
}
