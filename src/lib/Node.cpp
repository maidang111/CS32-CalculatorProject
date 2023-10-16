#include "AST.h"
#include "Node.h"
using namespace std;

Node::Node() {}

Node::~Node() {
}

Node* Node::switch_to_parent() const {
    return parent;
}

void Node::add_child(Node* child_node) {
    children.push_back(child_node);
}


