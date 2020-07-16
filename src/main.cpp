#include "Yocto.h"

void signal_handler( int signal_num ) { 
    std::cout << "Interrupt signal: " << signal_num << std::endl; 
    exit(signal_num);   
}

int main(int argc, const char * argv[]){
    signal(SIGSEGV, signal_handler);

    try{
        Yocto yocto(argc, argv);
        yocto.launch();
    }catch(YoctoException & e){
        std::cout << e.what() << std::endl;
    }catch(std::exception & e){
        std::cout << "Uncaught error: " << e.what() << std::endl;
    }

    return 0;
}