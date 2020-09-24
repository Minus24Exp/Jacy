#include "Jacy.h"
#include "Tester.h"

const bool RUN_TESTS = false;

void signal_handler(int signal_num){ 
    std::cout << "Interrupt signal: " << signal_num << std::endl;
    exit(signal_num);
}

int main(int argc, const char * argv[]){
    signal(SIGSEGV, signal_handler);
    signal(SIGINT, signal_handler);

    if (RUN_TESTS) {
        Tester & tester = Tester::get_instance();
        tester.run();
        return 0;
    }

    try {
        Jacy::get_instance().launch(argc, argv);
    } catch (JacyException & e) {
        std::cout << e.what() << std::endl;
    } catch(std::exception & e) {
        std::cout << "Uncaught error: " << e.what() << std::endl;
    }

    return 0;
}