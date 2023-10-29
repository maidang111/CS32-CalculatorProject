#include "AST.h"
#include "Node.h"
using namespace std;

Node::Node(Node* parent, Token* in_data, bool in_is_operator, bool equality_sign, bool is_variable) {
    this->parent = parent;
    this->data = in_data;
    this->is_operator = in_is_operator;
    this->equality_sign = equality_sign;
    this->is_variable = is_variable;
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
    // cout << "connect " << "parent: " << data->raw_value << " to child: " << (child_node->data)->raw_value << endl;
    children.push_back(child_node);
}

double Node::get_number() const{
    // this function returns error if the token raw_value cannot be converted to double
    return stod(data->raw_value);
}

string Node::return_operator() const {
    // call it only for operator although it does not return error if the data is double 
    return data->raw_value;
}

// AddNode::Node(Node* parent, Token* in_data, bool in_is_operator, bool equality_sign, bool is_variable){

// };

