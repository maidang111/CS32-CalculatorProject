#include <iostream>
#include "lib/Lexer.h"
#include "lib/InfixParser.h"
#include <string>

using namespace std; 

int main(){
    InfixParser infixParser;
    infixParser.read_all_token();
    infixParser.print_all();
    return 0;
}