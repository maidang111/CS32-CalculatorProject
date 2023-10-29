#include <iostream>
#include "AST.h"

using namespace std; 

Token::Token(){
    this->raw_value = "";
    this->column = -1;
    this->row = -1;
    this->value = 0;
};

size_t Token::get_value(){
    cout << "token type doesn't have a get value"  << endl;
    return 0;
}
void Token::print(){
    cout << "token type doesn't have a get print" << endl;
}

size_t Add::get_value(){
    return this->left->get_value() - this->right->get_value();
}

void Add::print(){
    cout << "(";
    this->left->print();
    cout << " + ";
    this->right->print();
    cout << ")";
} 

size_t Subtract::get_value(){
    return this->left->get_value() - this->right->get_value();
}
void Subtract::print(){
    cout << "(";
    this->left->print();
    cout << " - ";
    this->right->print();
    cout << ")";
} 

size_t Divide::get_value(){
    if ( this->right->get_value() != 0){
        return this->left->get_value() / this->right->get_value();
    } else {
        cout << "can't divide by 0" << endl;
        exit(1);
    }
}
void Divide::print(){
    cout << "(";
    this->left->print();
    cout << " / ";
    this->right->print();
    cout << ")";
} 

size_t Multiply::get_value(){
    return this->left->get_value() * this->right->get_value();
}
void Multiply::print(){
    cout << "(";
    this->left->print();
    cout << " * ";
    this->right->print();
    cout << ")";
} 

size_t Num::get_value(){
    return stod(this->raw_value);
}
void Num::print(){
    cout << this->raw_value;
}

size_t Variable::get_value(){
    return this->left->get_value() - this->right->get_value();
}
void Variable::print(){
    cout << this->raw_value;
}

size_t Equal::get_value(){
    return this->right->get_value();
}
void Equal::print(){
    cout << "(";
    this->left->print();
    cout << " = ";
    this->right->print();
    cout << ")";
} 

