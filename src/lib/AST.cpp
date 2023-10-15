#include <iostream>
#include "lib/AST.h"

using namespace std; 

Token::Token(){
    this->value = "";
    this->column = -1;
    this->row = -1;
};
