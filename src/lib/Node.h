#ifndef NODE_H
#define NODE_H
#include "AST.h"
#include <vector>

using namespace std;

class Node {
    protected:
    Node* parent;
    Token* data;

    public:
    Node();
    virtual ~Node();
    virtual bool node_type() const = 0;
    void add_child(Node* child_node);
    Node* switch_to_parent() const;
    virtual double get_number() const = 0;
    virtual string check_operator() const = 0;
    vector<Node*> children;

};

#endif
