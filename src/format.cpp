#include <iostream>
#include "lib/Lexer.h"
#include "lib/Formater.h"
#include <string>

using namespace std; 

int main(){
    Lexer lexer; 
    lexer.create_endtokens();
    if(lexer.error){
        exit(1);
    }
    lexer.print_endtokens();
    Formater formater(lexer.multi_end_tokens);
    formater.buildASTs();
    formater.printFormated();
    formater.deleteStatements();

    return 0;
}