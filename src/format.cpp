#include <iostream>
#include "lib/Lexer.h"
#include "lib/Formater.h"
#include <string>

using namespace std; 

int main(){
    Lexer lexer; 
    lexer.create_endtokens();
    Formater formater(lexer.multi_end_tokens);

    formater.buildASTs();
    formater.printFormated();
    lexer.delete_endtokens();
    // cout << formater.ASTHeads.size();
    // formater.deleteStatements();

    return 0;
}