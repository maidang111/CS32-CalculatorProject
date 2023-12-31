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
    bool error;
    vector <string> whole_input;
    set <char> possible_values;
    set <char> inequalities;
    set <char> logicals;
    vector <Token*> tokens;
    vector <Token*> multi_end_tokens;
    vector<vector<Token*>> data;

    ~Lexer();

    void print_tokens();
    void print_endtokens();
    void help_delete_data();
    vector<Token*> create_one_line_tokens(size_t line_index);
    void create_tokens();
    void create_endtokens();

    void delete_tokens();
    void delete_endtokens();
};

#endif
