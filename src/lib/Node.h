#ifndef NODE_H
#define NODE_H
#include "AST.h"
#include <vector>

using namespace std;

class Node {
    private:
    Node* parent;
    Token* data;
    bool is_operator;

    public:
    Node(Node* parent, Token* in_data, bool in_is_operator);
    ~Node();
    bool node_type() const;
    void add_child(Node* child_node);
    Node* switch_to_parent() const;
    double get_number() const;
    string return_operator() const;
    vector<Node*> children;
};

#endif
