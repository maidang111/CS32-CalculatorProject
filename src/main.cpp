#include <iostream>
#include "lib/Lexer.h"
#include "lib/Parser.h"
#include <string>

using namespace std; 

int main(){
    string user_input = "";
    cin >> user_input;

    Lexer lexer(user_input);
    lexer.create_tokens();

    return 0;
}