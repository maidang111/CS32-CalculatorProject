#include <iostream>
#include "lib/Lexer.h"
#include "lib/InfixParser.h"
#include <string>

using namespace std; 

int main(){
    Lexer a;
    a.create_endtokens();
    InfixParser infixParser(a.multi_end_tokens);
    infixParser.read_all_token();
    infixParser.print_all();
    return 0;
}