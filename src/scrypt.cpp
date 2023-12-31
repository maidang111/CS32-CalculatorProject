#include <iostream>
#include "lib/Lexer.h"
#include "lib/Scrypter.h"
#include <string>

using namespace std; 

int main(){
    Lexer lexer; 
    lexer.create_endtokens();
    if(lexer.error){
        exit(1);
    }
    // lexer.print_endtokens();
    Scrypter scrypt(lexer.multi_end_tokens);
    scrypt.check_return();
    scrypt.buildASTs();
    scrypt.calculate();
    // // lexer.delete_endtokens();
    // // cout << formater.ASTHeads.size();
    scrypt.deleteStatements();
    return 0;
}