#include <iostream>
#include "AST.h"

using namespace std; 

Token::Token(){
    this->raw_value = "";
    this->column = -1;
    this->row = -1;
    this->value = 0;
};
void Token::get_value(){
    cout << "token type doesn't have a get value"  << endl;
}
void Token::print(){
    cout << "token type doesn't have a get print" << endl;
}

void Add::get_value(){
    this->value = this->left->value + this->right->value;
};

void Add::print(){
    cout << "(";
    this->left->print();
    cout << " + ";
    this->right->print();
    cout << ")";
} 

void Subtract::get_value(){
    this->value = this->left->value - this->right->value;
}
void Subtract::print(){
    cout << "(";
    this->left->print();
    cout << " - ";
    this->right->print();
    cout << ")";
} 

void Divide::get_value(){
    if (this->right->value != 0){
        this->value = this->left->value / this->right->value;
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

void Multiply::get_value(){
    this->value = this->left->value * this->right->value;
}
void Multiply::print(){
    cout << "(";
    this->left->print();
    cout << " * ";
    this->right->print();
    cout << ")";
} 

void Num::get_value(){
    this->value = stod(this->raw_value);
}
void Num::print(){
    cout << this->raw_value;
}

void Variable::get_value(){
    this->value = stod(this->raw_value);
}
void Variable::print(){
    cout << this->raw_value;
}

void Equal::get_value(){
    this->value = this->right->value;
}
void Equal::print(){
    cout << "(";
    this->left->print();
    cout << " = ";
    this->right->print();
    cout << ")";
} 

