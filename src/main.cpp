#include "Yocto.h"
#include "Tester.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

const bool RUN_TESTS = true;

void signal_handler(int signal_num){ 
    std::cout << "Interrupt signal: " << signal_num << std::endl; 
    exit(signal_num);   
}

int main(int argc, const char * argv[]){
    struct sigaction act;
    act.sa_handler = signal_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);

    if(RUN_TESTS){
        Tester & tester = Tester::get_instance();
        tester.run();
        return 0;
    }

    try{
        Yocto::get_instance().launch(argc, argv);
    }catch(YoctoException & e){
        std::cout << e.what() << std::endl;
    }catch(std::exception & e){
        std::cout << "Uncaught error: " << e.what() << std::endl;
    }

    return 0;
}