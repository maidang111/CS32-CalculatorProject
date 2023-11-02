
#include <iostream>
#include "AST.h"
#include <set>

using namespace std; 

Token::Token(){
    this->raw_value = "";
    this->column = -1;
    this->row = -1;
    this->left = nullptr;
    this->right = nullptr;
    this->error_ = false;
};

bool Token::error_ = false;
map<string,Token*> Token::variable_update;
map<string, double> Token::variable_value;
set<string> Token::variable_list;


void Token::delete_token(Token* node){
    if (!node) {
        return;
    }
    delete_token(node->left);
    delete_token(node->right);
    delete node; 
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
        variable_list.insert(left->raw_value);
        if (variable_update.find(left->raw_value) == variable_update.end()) {
            variable_update.emplace(left->raw_value, left);
        }
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
    if (variable_list.find(raw_value) == variable_list.end()) {
        cout << "Runtime error: unknown identifier " << raw_value << endl;
        error_ = true;
    }
    if (error_) {
        // cout << " error " << endl;
        if (!variable_value.empty()) {
            // cout << 1 << endl;
            for (auto& var: variable_update) {
                var.second->value = variable_value.at(var.first);
            }
        }
    }
    return this->value;
}
void Variable::print(){
    cout << this->raw_value;
}