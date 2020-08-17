#include "Tester.h"

Tester::Tester()
    : lexer(Lexer::get_instance()),
      parser(Parser::get_instance())
{
    prepare();
}

void Tester::prepare(){
    std::cout << "[Prepairing Tests]\n\n";

    struct dirent * entry;
    DIR * tests_dir = opendir("test");

    if(tests_dir == NULL){
        throw YoctoException("Unable to open \"test\" folder");
    }

    // Collect tests
    while((entry = readdir(tests_dir)) != NULL){
        // Skip `.`, `..` and maybe hidden files
        if(entry->d_name[0] != '.'){
            test_list.push_back("test/"+ std::string(entry->d_name));
        }
    }

    closedir(tests_dir);
}

void Tester::run(){
    std::cout << "\n[Run tests]" << std::endl;
    for(const auto & test : test_list){
        bool passed = run_test(test);
        if(!passed && !continue_after_fail){
            std::cout << "[\u001b[31mStopped after fail\u001b[0m]" << std::endl;
            return;
        }
    }
}

std::string Tester::read_file(const std::string & path){
    std::fstream file(path);

    if(!file.is_open()){
        throw FileNotFoundException(path);
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    return ss.str();
}

bool Tester::run_test(const std::string & path){
    std::string error_msg;

    std::string script;
    TokenStream tokens;
    StmtList tree;
    try{
        script = read_file(path);

        tokens = lexer.lex(script);

        tree = parser.parse(tokens);
    }catch(YoctoException & e){
        error_msg = e.what();
    }

    std::string message;
    if(error_msg.empty()){
        // If test checked
        message = "(\u001b[32m✔\u001b[0m) Test `" + path + "` passed";
        std::cout << message << std::endl;
    }else{
        message = "(\u001b[31m×\u001b[0m) Test `" + path + "` not passed";
        std::cout << message << std::endl;
        
        std::cout << "\tError: " << error_msg << std::endl;

        if(tokens.size() > 0){
            std::cout << "Token:" << std::endl;
            for(auto & t : tokens){
                std::cout << t.to_string() << std::endl;
            }
        }

        if(tree.size() > 0){
            Printer printer;
            std::cout << "Parse Tree:" << std::endl;
            printer.print(tree);
            std::cout << std::endl;
        }
    }

    // Pretty printed delimiter
    // -10 because of ANSI colors in message
    for(int i = 0; i < message.size() - 10; i++){
        std::cout << "—";
    }
    std::cout << "\n\n";

    return error_msg.empty();
}