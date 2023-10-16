#include "AST.h"
#include "Number.h"

using namespace std;

Number::Number(Node* parent_token, Token* in_token) {
    parent = parent_token;
    number_value = stod(in_token->value);
}

bool Number::node_type() const {
    return false;
}

double Number::get_number() const{ return number_value;}

Number::~Number() {
}