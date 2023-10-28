#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "Node.h"
#include "Lexer.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class Parser {
    private:
    vector<Node*> roots;
    void print_help(Node* in_node, bool parenthesis);
    double calculate_help(Node* operator_node);
    void delete_help(Node* current_node);
    vector<bool> first_parenthesis; // 
    bool check_double(string s) const;
    map<string, double> variables;
    map<string, bool> known_variables;
    public:
    Parser();
    Node* read_tokens(vector<Token*>& tokens_list, Node* new_node);
    void print();

    double calculate(Node* root);
    void print_error_2(Token* error_token);
    void read_all_lines(Lexer in_lexer);
    ~Parser();


};

#endif
