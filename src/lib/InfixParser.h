#ifndef INFIXPARSER_H
#define INFIXPARSER_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "AST.h"
#include "AST_Node.h"
#include "Data.h"

using namespace std; 

class InfixParser{
    public:
    vector<Token*> tokens;
    vector<AST_Node*> ASTs;
    size_t index;
    bool error;
    
    InfixParser();
    ~InfixParser();
    void read_all_token();
    void read_token();
    AST_Node* read_one_line(size_t begin_line, size_t end_line, AST_Node* in_parent);
    void delete_help(AST_Node* in_node);
    void print_all();
    void print_AST(AST_Node* node) const;
    double result_double;
    bool result_bool;
    Data evaluate(AST_Node* in_node);
    void evaluate_print(AST_Node* head); 
    void update_variables();
    vector<size_t> error_index;
    void check_parenthesis(size_t first_element, size_t last_element); 
    void check_operator(size_t first_element, size_t last_element);
    set<string> operators;
    // build tree
    // A print function
    // calculate function
};

#endif