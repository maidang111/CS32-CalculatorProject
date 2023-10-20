#include "AST.h"
#include "Node.h"
using namespace std;

Node::Node(Node* parent, Token* in_data, bool in_is_operator) {
    this->parent = parent;
    this->data = in_data;
    this->is_operator = in_is_operator;
}

Node::~Node() {
}

bool Node::node_type() const {
    return is_operator;
}

Node* Node::switch_to_parent() const {
    return parent;
}

void Node::add_child(Node* child_node) {
    // cout << "connect " << "parent: " << data->value << " to child: " << (child_node->data)->value << endl;
    children.push_back(child_node);
}

double Node::get_number() const{
    // this function returns error if the token value cannot be converted to double
    return stod(data->value);
}

string Node::return_operator() const {
    // call it only for operator although it does not return error if the data is double 
    return data->value;
}

