
#include <iostream>
#include "AST.h"
#include <cmath>
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
bool Token::outside_ = false;
map<string,Token*> Token::variable_update;
map<string, double> Token::variable_value;
set<string> Token::variable_list;

void Token::set_type(string val) {
    if (val == "true" || val == "false") {
        data_type = BOOL;
    }
    else {
        data_type = DOUBLE;
    }
}

void Token::set_type(Token* assign) {
    if (assign->data_type == BOOL) {
        data_type = BOOL;
    }
    else {
        data_type = DOUBLE;
    }
}

void Token::delete_token(Token* node){
    if (!node) {
        return;
    }
    delete_token(node->left);
    delete_token(node->right);
    delete node; 
}

Token::~Token() {
    // cout << "deleting value: " << raw_value << endl;
}

void Token::set_token_type(string a) {
    if (a == "bool" || a == "true" || a == "false") {
        data_type = BOOL;
    }
    else if (a == "end") {
        data_type = END;
    }
    else {
        data_type = DOUBLE;
    }
}

string Token::return_type() const {
    if (data_type == BOOL) {
        return "bool";
    }
    else if (data_type == DOUBLE) {
        return "double";
    }
    return "end";
}

double Token::get_value(){
    cout << "token type doesn't have a get value"  << endl;
    return 0;
}
void Token::print(){
    cout << "token type doesn't have a get print" << endl;
}

bool Logical::get_value_bool(){
    if (this->left != nullptr && this->right != nullptr){
        if (this->left->data_type != BOOL || this->right->data_type != BOOL) {
            //error
            cout << "Runtime error: invalid operand type." << endl;
            error_ = true;
            // go back and reupdate variables
            return 0;
        }
        bool left_val = left->get_value_bool();
        bool right_val = right->get_value_bool();
        if (raw_value == "|") {
            return (left_val || right_val);
        }
        else if (raw_value == "^") {
            if (left_val && !right_val || !left_val && right_val){
                return true;
            }
            return false;
        }
        else if (raw_value == "&") {
            return left_val && right_val;
        }
    }
    return false;
}

void Logical::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " " << raw_value << " ";
        this->right->print();
        cout << ")";
    }
} 

bool Equality::get_value_bool(){
    if (this->left != nullptr && this->right != nullptr){
        if (this->left->data_type != this->right->data_type) {
            //error
            cout << "Runtime error: invalid operand type." << endl;
            error_ = true;
            // go back and reupdate variables
            return 0;
        }
        if (raw_value == "==") {
            return left->raw_value == right->raw_value;
        }
        else if (raw_value == "!=") {
            return left->raw_value != right->raw_value;
        }
    }
    return false;
}

void Equality::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " " << raw_value << " ";
        this->right->print();
        cout << ")";
    }
} 

bool Comparison::get_value_bool(){
    if (this->left != nullptr && this->right != nullptr){
        if (this->left->data_type != DOUBLE || this->right->data_type != DOUBLE) {
            //error
            cout << "Runtime error: invalid operand type." << endl;
            error_ = true;
            // go back and reupdate variables
            return 0;
        }
        if (raw_value == "<") {
            return left->get_value() < right->get_value();
        }
        else if (raw_value == "<=") {
            return left->get_value() <= right->get_value();
        }
        else if (raw_value == ">") {
            return left->get_value() > right->get_value();
        }
        else if (raw_value == ">=") {
            return left->get_value() >= right->get_value();
        }
    }
    return false;
}
void Comparison::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " " << raw_value << " ";
        this->right->print();
        cout << ")";
    }
} 

double Add::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        if (this->left->data_type != this->right->data_type) {
            //error
            cout << "Runtime error: invalid operand type." << endl;
            error_ = true;
            // go back and reupdate variables
            return 0;
        }
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
        if (this->left->data_type != this->right->data_type) {
            //error
            cout << "Runtime error: invalid operand type." << endl;
            error_ = true;
            // go back and reupdate variables
            return 0;
        }
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
    if (this->left->data_type != this->right->data_type) {
        //error
        cout << "Runtime error: invalid operand type." << endl;
        error_ = true;
        // go back and reupdate variables
        return 0;
    }
    if (this->right->get_value() != 0){
        return this->left->get_value() / this->right->get_value();
    } else {
        cout << "Runtime error: division by zero." << endl;
        error_ = true;
        return 0;
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

// done for mode
double Mode::get_value(){
    if (this->left == nullptr || this->right == nullptr){
        return 0;
    }
    if (this->right->get_value() != 0){
        return fmod(this->left->get_value(), this->right->get_value());
    } else {
        cout << "Runtime error: division by zero." << endl;
        error_ = true;
        return 0;
    }
}
void Mode::print(){
    if (this->left != nullptr && this->right != nullptr){
        cout << "(";
        this->left->print();
        cout << " % ";
        this->right->print();
        cout << ")";
    }
} 

double Multiply::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        if (this->left->data_type != this->right->data_type) {
            //error
            cout << "Runtime error: invalid operand type." << endl;
            error_ = true;
            // go back and reupdate variables
            return 0;
        }
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

bool Equal::get_value_bool() {

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
    cout << this->get_value();
}

double Variable::get_value(){
    if (variable_list.find(raw_value) == variable_list.end() && !outside_) {
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

bool Variable::get_value_bool() {
    if (variable_list.find(raw_value) == variable_list.end() && !outside_) {
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
        return false;
    }
    return bool_val;
}

void Variable::print(){
    cout << this->raw_value;
}
