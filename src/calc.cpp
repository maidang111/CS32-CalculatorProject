#include <iostream>
#include "lib/Lexer.h"
#include "lib/InfixParser.h"
#include <string>

using namespace std; 

int main(){
    Lexer lexer;
    lexer.create_endtokens();
    InfixParser infixParser(lexer.multi_end_tokens);
    infixParser.build_AST();
    lexer.delete_endtokens();
    infixParser.delete_tokens();
    return 0;
}