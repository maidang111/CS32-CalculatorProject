#include <iostream>
#include "lib/Lexer.h"
#include "lib/InfixParser.h"
#include "lib/AST.h"
#include <string>

using namespace std; 

int main(){
    Lexer a;
    a.create_endtokens();
    for (size_t i = 0; i < a.multi_end_tokens.size(); ++i) {
        cout << a.multi_end_tokens.at(i)->raw_value << endl;
    }
    // InfixParser infixParser(a.multi_end_tokens);
    // infixParser.read_all_token();
    // infixParser.print_all();
    return 0;
}