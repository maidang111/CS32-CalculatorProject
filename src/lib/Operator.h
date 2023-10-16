#ifndef OPERATOR_H
#define OPERATOR_H
#include "AST.h"
#include "Node.h"
#include <vector>

using namespace std;

class Operator: public Node {
    char operator;

    public: 
    Operator(Node* parent_node, Token* in_token);
    void add_child(Node* in_child);
    bool node_type() const;
    char check_operator() const;
    vector<Node*> children;
    ~Operator();
};

#endif