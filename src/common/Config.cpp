#include "common/Config.h"

namespace jc::common {
    std::map<std::string, CLArg> Config::cl_args = {
        {"debug", {true, "0"}},
        {"run_level", {false, "vm"}},
    };

    void Config::config(int argc, const char ** argv) {
        // jacy_args -> false when main file appears
        bool jacy_args = true;

        std::vector<std::string> script_argv;
        for (int i = 1; i < argc; i++) {
            std::string arg(argv[i]);

            if (arg[0] == '-') {
                if (jacy_args) {
                    // Parse Jacy arguments
                    const auto & cl_arg = cl_args.find(arg.substr(1));
                    if (cl_arg != cl_args.end()) {
                        if (cl_arg->second.flag) {
                            cl_arg->second.value = "1";
                        } else {
                            cl_arg->second.value = argv[i + 1];
                            i++;
                        }
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

        // Set jacy args
        if (cl_args.at("debug").value == "1") {
            jacy_options.debug = true;
        }

        const auto & run_level = cl_args.at("run_level").value;
        if (run_level == "lexer") {
            jacy_options.run_level = RunLevel::Lexer;
        } else if (run_level == "parser") {
            jacy_options.run_level = RunLevel::Parser;
        } else if (run_level == "print_tree") {
            jacy_options.run_level = RunLevel::PrintTree;
        } else if (run_level == "compiler") {
            jacy_options.run_level = RunLevel::Compiler;
        } else if (run_level == "disasm") {
            jacy_options.run_level = RunLevel::Disasm;
        } else {
            jacy_options.run_level = RunLevel::Vm;
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
