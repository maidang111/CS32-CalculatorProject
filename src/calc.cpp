#include <iostream>
#include "lib/Lexer.h"
#include "lib/InfixParser.h"
#include <string>

using namespace std; 

int main(){
    Lexer lexer;
    lexer.create_endtokens();
    InfixParser infixParser(lexer.multi_end_tokens);
    // cout << infixParser.tokens.size();
    infixParser.build_AST();
    // cout << lexer.multi_end_tokens.size() << endl;
    infixParser.delete_tokens();
    infixParser.tokens.clear();
    return 0;
}