#include <iostream>
#include "lib/Lexer.h"
#include "lib/InfixParser.h"
#include "lib/AST.h"
#include <string>

using namespace std; 

int main(){
    Lexer a;
    a.create_endtokens();
    InfixParser infixParser(a.multi_end_tokens);
    infixParser.read_all_token(true);
    infixParser.print_all();
    a.delete_endtokens();
    return 0;
}