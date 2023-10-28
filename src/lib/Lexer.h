#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <string>
#include <vector>
#include "AST.h"

using namespace std; 

class Lexer{
    public:
    vector <char> user_input;
    Lexer();
    unsigned int num_line;

    vector <string> whole_input;
    set <char> possible_values;
    vector <Token*> tokens;
    ~Lexer();

    void create_tokens();
    void print_tokens();
    void delete_tokens();
};

#endif
