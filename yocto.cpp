#include "src/Yocto.h"

int main(int argc, const char * argv[]){

	try{
		Yocto yocto;

		if(argc == 1){
			yocto.run_prompt();
		}else if(argc == 2){
			const std::string path(argv[1]);
			yocto.run_script(path);
		}else{
			throw IllegalArgumentException("Expected only one argument");
		}

	}catch(YoctoException & e){
		std::cout << e.what() << std::endl;
	}catch(std::exception & e){
		std::cout << "Uncaught error: " << e.what() << std::endl;
	}

	return 0;
}