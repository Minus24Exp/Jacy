#include "Jacy.h"

const bool RUN_TESTS = false;

#define IS_WIN defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#if IS_WIN
void signal_handler(int signal_num) { 
    std::cout << "Interrupt signal: " << signal_num << std::endl;
    exit(signal_num);
}
#endif

int main(int argc, const char * argv[]) {
    #if IS_WIN
    signal(SIGSEGV, signal_handler);
    signal(SIGINT, signal_handler);
    #endif

    try {
        jc::Jacy::get_instance().launch(argc, argv);
    } catch (jc::JacyException & e) {
        std::cout << e.what() << std::endl;
    } catch (std::exception & e) {
        std::cout << "Uncaught error: " << e.what() << std::endl;
    }

    return 0;
}