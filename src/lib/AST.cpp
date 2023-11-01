
#include <iostream>
#include "AST.h"

using namespace std; 

Token::Token(){
    this->raw_value = "";
    this->column = -1;
    this->row = -1;
    this->left = nullptr;
    this->right = nullptr;
};
void Token::deleteToken(){
    if(this->left != nullptr){
        this->left->deleteToken();
        this->left = nullptr;
    }
    if(this->right != nullptr){
        this->right->deleteToken();
        this->right = nullptr;
    }
    delete this; 
}

double Token::get_value(){
    cout << "token type doesn't have a get value"  << endl;
    return 0;
}
void Token::print(){
    cout << "token type doesn't have a get print" << endl;
}

double Add::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        return this->left->get_value() + this->right->get_value();
    }
    return 0;
}

void Add::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " + ";
        this->right->print();
        cout << ")";
    }
} 

double Subtract::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        return this->left->get_value() - this->right->get_value();
    }
    return 0;
}
void Subtract::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " - ";
        this->right->print();
        cout << ")";
    }
} 

double Divide::get_value(){
    if (this->left == nullptr || this->right == nullptr){
        return 0;
    }
    if ( this->right->get_value() != 0){
        return this->left->get_value() / this->right->get_value();
    } else {
        throw runtime_error(" Runtime error: division by zero.");
    }
}
void Divide::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " / ";
        this->right->print();
        cout << ")";
    }
} 

double Multiply::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        return this->left->get_value() * this->right->get_value();
    }
    return 0;
}
void Multiply::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " * ";
        this->right->print();
        cout << ")";
    }
} 

double Equal::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        this->left->value = this->right->get_value();
        return this->right->get_value();
    }
    return 0;
}
void Equal::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " = ";
        this->right->print();
        cout << ")";
    }
} 

double Num::get_value(){
    return stod(this->raw_value);
}
void Num::print(){
    cout << this->raw_value;
}

double Variable::get_value(){
    return this->value;
}
void Variable::print(){
    cout << this->raw_value;
}