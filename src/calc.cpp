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
    a.delete_endtokens();
    // for (size_t i = 0; i < infixParser.tokens.size(); ++i) {
    //     if (infixParser.tokens.at(i)->raw_value == "END") {
    //         cout << endl;
    //     }
    //     else {
    //         cout << infixParser.tokens.at(i)->raw_value;
    //     }
    // }
    return 0;
}