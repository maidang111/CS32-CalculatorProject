#include <iostream>
#include <string>
#include <vector>
#include "AST.h"

using namespace std; 

class Lexer{
    public:
    vector <char> user_input;
    Lexer();

    vector <string> whole_input;
    set <char> possible_values;
    vector <Token*> tokens;
    ~Lexer();

    void create_tokens();
    void print_tokens();
};
