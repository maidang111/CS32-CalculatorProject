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
    set <char> inequalities;
    vector <Token*> tokens;
    vector <Token*> multi_end_tokens;

    ~Lexer();

    void print_tokens();
    void print_endtokens();
    
    void create_tokens();
    void create_endtokens();

    void delete_tokens();
    void delete_endtokens();
};

#endif
