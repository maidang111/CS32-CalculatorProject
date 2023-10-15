#include <iostream>
#include "lib/Token.h"

using namespace std; 

Token::Token(){
    this->value = "";
    this->column = -1;
    this->row = -1;
};
