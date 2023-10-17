#include <iostream>
#include "lib/Parser.h"

using namespace std; 

int main() {
    string user_input = "";
    Lexer lexer;
    Parser parser;

    lexer.create_tokens();
    parser.read_tokens(lexer.tokens);
    parser.print();

    lexer.delete_tokens();



    return 0;
}
