#include <iostream>
#include "lib/Lexer.h"
#include "lib/InfixParser.h"
#include "lib/AST.h"
#include <string>

using namespace std; 

int main(){
    Lexer a;
    // a.create_endtokens();
    InfixParser infixParser;
    vector<Token*> line;
    for (size_t i = 0; i < a.whole_input.size(); ++i) {
        line = a.create_one_line_tokens(i);
        if (line.size() == 1) {
            break;
        }
        if (!a.error) {
            infixParser.tokens = line;
            infixParser.read_token(true);
            infixParser.index = 0;
        }
    }
    a.help_delete_data();
    // infixParser.read_all_token(true);
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