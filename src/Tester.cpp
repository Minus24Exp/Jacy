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
    DIR * lexer_tests_dir = opendir("test/lexer");
    DIR * parser_tests_dir = opendir("test/parser");

    if(lexer_tests_dir == NULL){
        throw YoctoException("Unable to open \"test/lexer\" folder not found");
    }

    if(parser_tests_dir == NULL){
        throw YoctoException("Unable to open \"test/parser\" folder not found");
    }

    // Collect lexer tests
    while((entry = readdir(lexer_tests_dir)) != NULL){
        // Skip `.`, `..` and maybe hidden files
        if(entry->d_name[0] != '.'){
            lexer_test_list.push_back("test/lexer/"+ std::string(entry->d_name));
        }
    }

    // Collect parser tests
    while((entry = readdir(parser_tests_dir)) != NULL){
        // Skip `.`, `..` and maybe hidden files
        if(entry->d_name[0] != '.'){
            parser_test_list.push_back("test/parser/"+ std::string(entry->d_name));
        }
    }

    closedir(lexer_tests_dir);
    closedir(parser_tests_dir);
}

void Tester::run(){
    std::cout << "\n[Run lexer tests]" << std::endl;
    for(const auto & lexer_test : lexer_test_list){
        run_test(lexer_test, TestType::Lexer);
    }

    std::cout << "\n[Run parser tests]" << std::endl;
    for(const auto & parser_test : parser_test_list){
        run_test(parser_test, TestType::Parser);
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

void Tester::run_test(const std::string & path, TestType test_type){
    std::string error_msg;

    std::string script;
    TokenStream tokens;
    StmtList tree;
    try{
        script = read_file(path);

        tokens = lexer.lex(script);

        if(test_type == TestType::Parser){
            parser.parse(tokens);
        }
    }catch(YoctoException & e){
        error_msg = e.what();
    }

    if(error_msg.empty()){
        // If test checked
        std::cout << "(\u001b[32m✔\u001b[0m) Test `" << path << "` passed" << std::endl;
    }else{
        std::cout << "(\u001b[31m×\u001b[0m) Test `" << path << "` not passed" << std::endl;
        
        std::cout << "\tError: " << error_msg << std::endl;
        
        std::cout << "Code:" << std::endl;
        std::cout << script << std::endl;

        if(tokens.size() > 0){
            std::cout << "Token:" << std::endl;
            for(auto & t : tokens){
                std::cout << t.to_string() << std::endl;
            }
        }

        if(tree.size() > 0 && test_type == TestType::Parser){
            Printer printer;
            std::cout << "Parse Tree:" << std::endl;
            printer.print(tree);
            std::cout << std::endl;
        }
    }

    std::cout << "——————————————————————————————————————————————————————————————————————————\n\n";
}