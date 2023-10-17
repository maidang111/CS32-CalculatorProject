#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "Node.h"
#include <string>
using namespace std;

class Parser {
    private:
    Node* root;
    string print_help(Node* in_node) const;
    double calculate_help(Node* operator_node) const;
    void delete_help(Node* current_node);
    public:
    Parser();
    void read_tokens(vector<Token*> tokens_list);
    void print() const;
    double calculate() const;
    void print_error_2() const;
    ~Parser();


};

#endif
