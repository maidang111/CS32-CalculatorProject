#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "Node.h"
#include <string>
using namespace std;

class Parser {
    private:
    Node* root;
    void print_help(Node* in_node, bool parenthesis);
    double calculate_help(Node* operator_node);
    void delete_help(Node* current_node);
    bool first_parenthesis;
    public:
    Parser();
    void read_tokens(vector<Token*> tokens_list);
    void print();
    double calculate();
    void print_error_2(Token* error_token);
    ~Parser();


};

#endif
