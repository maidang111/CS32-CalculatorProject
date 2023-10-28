#ifndef NODE_H
#define NODE_H
#include "AST.h"
#include <vector>

using namespace std;

class Node {
    private:
    Node* parent;
    bool is_operator;

    public:
    Node(Node* parent, Token* in_data, bool in_is_operator, bool equality_sign, bool is_variable);
    ~Node();
    bool is_variable;
    bool equality_sign;
    Token* data;
    bool node_type() const;
    void add_child(Node* child_node);
    Node* switch_to_parent() const;
    double get_number() const;
    string return_operator() const;
    vector<Node*> children;
};

#endif

