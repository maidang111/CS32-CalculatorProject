#include "Operator.h"
#include "Node.h"
#include "AST.h"
#include <vector>
using namespace std;

Operator::Operator (Node* parent_node, Token* in_token) {
    operator = in_token->value;
    parent = parent_node;
}

void Operator::add_child(Node* in_child) {
    children.push_back(in_child);
}

bool Operator::node_type() const {
    return true;
}

char Operator::check_operator() const {
    return operator;
}

Operator::~Operator() {}