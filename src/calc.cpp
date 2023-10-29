#include <iostream>
#include "lib/Lexer.h"
#include "lib/InfixParser.h"
#include <string>

using namespace std; 

int main(){
    InfixParser infixParser;
    infixParser.build_AST();
    return 0;
}