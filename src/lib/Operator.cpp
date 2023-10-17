#include "Operator.h"
#include "Node.h"
#include "AST.h"
#include <vector>
using namespace std;

Operator::Operator(Node* parent_node, Token* in_token) : Node() {
    operator_value = in_token->value;
    data = in_token;
    parent = parent_node;
}



bool Operator::node_type() const {
    return true;
}

string Operator::check_operator() const {
    return operator_value;
}

Operator::~Operator() {}

double Operator::get_number() const {
    return 0;
}

