#include <iostream>
#include "lib/Lexer.h"
#include "lib/Parser.h"
#include <string>

using namespace std; 

int main(){
    string user_input = "";
    Lexer lexer;
    lexer.create_tokens();
    lexer.print_tokens();
    lexer.delete_tokens();
    return 0;
}