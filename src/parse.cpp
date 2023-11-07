#include <iostream>
#include <string>
#include <vector>
#include "lib/AST.h"
#include "lib/Parser.h"
#include "lib/Lexer.h"

using namespace std; 

int main() {
    string user_input = "";
    Lexer lexer;
    Parser parser;

    lexer.create_tokens();
    parser.read_all_lines(lexer);
    // cout << "Done reading" << endl;
    parser.print();

    // lexer.delete_tokens();
    return 0;
}
