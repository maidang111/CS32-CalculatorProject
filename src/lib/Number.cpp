#include "AST.h"
#include "Number.h"
#include <string>

using namespace std;

Number::Number(Node* parent_token, Token* in_token) : Node() {
    number_value = stod(in_token->value);
    parent = parent_token;
    data = in_token;
}

bool Number::node_type() const {
    return false;
}

double Number::get_number() const{ return number_value;}

Number::~Number() {
}