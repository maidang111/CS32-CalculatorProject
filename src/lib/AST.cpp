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
bool Token::outside_ = false;
map<string,Token*> Token::variable_update;
map<string, double> Token::variable_value;
map<string, bool> Token::variable_bool;
set<string> Token::variable_list;

string Token::get_data_type() const {
    if (data_type == BOOL) {
        return "BOOL";
    }
    else if (data_type == DOUBLE) {
        return "DOUBLE";
    }
    return "END";
}

void Token::print_invalid_type() const {
    cout << "Runtime error: invalid operand type." << endl;
    error_ = true;
}

void Token::set_type(string val) {
    if (val == "true" || val == "false") {
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

variant<bool, double> Token::get_value(){
    cout << "token type doesn't have a get value"  << endl;
    return 0.0;
}
void Token::print(){
    cout << "token type doesn't have a get print" << endl;
}

variant<bool, double> Add::get_value(){
    // always number 
    if (this->left != nullptr && this->right != nullptr){
        if ((this->left->data_type != DOUBLE) && (this->right->data_type != DOUBLE)) {
            print_invalid_type();
            // reupdate variable if needed
            return 0.0;
        }
        if (holds_alternative<double>(left->get_value()) && holds_alternative<double>(right->get_value())) {
            double lf = get<double>(left->get_value());
            double r = get<double>(right->get_value());
            return lf + r;
        }    
    }
    return 0.0;
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

variant<bool, double> Subtract::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        if ((this->left->data_type != DOUBLE) && (this->right->data_type != DOUBLE)) {
            print_invalid_type();
            // reupdate variable if needed
            return 0.0;
        }
        if (holds_alternative<double>(left->get_value()) && holds_alternative<double>(right->get_value())) {
            double lf = get<double>(left->get_value());
            double r = get<double>(right->get_value());
            return lf - r;
        }    
    }
    return 0.0;
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

variant<bool, double> Divide::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        if ((this->left->data_type != DOUBLE) && (this->right->data_type != DOUBLE)) {
            print_invalid_type();
            // reupdate variable if needed
            return 0.0;
        }
        if (holds_alternative<double>(left->get_value()) && holds_alternative<double>(right->get_value())) {
            double lf = get<double>(left->get_value());
            double r = get<double>(right->get_value());
            if (r != 0.0) {
                return lf / r;
            }
            else {
                cout << "Runtime error: division by zero." << endl;
                error_ = true;
                return 0.0;
            }
        }    
    }
    return 0.0;
    // if (this->left == nullptr || this->right == nullptr){
    //     return 0.0;
    // }
    // if ( this->right->get_value() != 0.0){
    //     return this->left->get_value() / this->right->get_value();
    // } else {
    //     cout << "Runtime error: division by zero." << endl;
    //     error_ = true;
    //     return 0.0;
    // }
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

variant<bool, double> Multiply::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        if ((this->left->data_type != DOUBLE) && (this->right->data_type != DOUBLE)) {
            print_invalid_type();
            // reupdate variable if needed
            return 0.0;
        }
        if (holds_alternative<double>(left->get_value()) && holds_alternative<double>(right->get_value())) {
            double lf = get<double>(left->get_value());
            double r = get<double>(right->get_value());
            return lf * r;
        }    
    }
    return 0.0;
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

// done
variant<bool, double> Equal::get_value(){
    if (this->left != nullptr && this->right != nullptr){
        left->data_type = right->data_type;
        if (holds_alternative<double>(right->get_value())) {
            this->left->value = get<double>(right->get_value());
            left->data_type = DOUBLE;
        }
        else if (holds_alternative<double>(right->get_value())) {
            this->right->bool_val = get<bool>(right->get_value());
            left->data_type = BOOL;
        }
        // this->left->value = this->right->get_value();
        variable_list.insert(left->raw_value);
        if (variable_update.find(left->raw_value) == variable_update.end()) {
            variable_update.emplace(left->raw_value, left);
        }
        return this->right->get_value();
    }
    return 0.0;
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

variant<bool, double> Num::get_value(){
    return stod(this->raw_value);
}

void Num::print(){
    cout << value;
}

variant<bool, double> Variable::get_value(){
    if (variable_list.find(raw_value) == variable_list.end() && !outside_) {
        cout << "Runtime error: unknown identifier " << raw_value << endl;
        error_ = true;
    }
    if (error_) {
        // cout << " error " << endl;
        if (!variable_value.empty()) {
            // cout << 1 << endl;
            // need to edit here
            for (auto& var: variable_update) {
                var.second->value = variable_value.at(var.first);
            }
        }
    }
    if (data_type == BOOL) {
        return bool_val;
    }
    return value;
}

// done
void Variable::print(){
    cout << this->raw_value;
}

variant<bool, double> Comparison::get_value() {
    if (left && right) {
        if ((left->data_type != DOUBLE) || (right->data_type != DOUBLE)) {
            print_invalid_type();
            // variable update needed here
            return 0.0;
        }
        if (holds_alternative<double>(left->get_value()) && holds_alternative<double>(right->get_value())) {
            double a = get<double>(left->get_value());
            double b = get<double>(right->get_value());
            if (raw_value == "<") {
                return a < b;
            }
            else if (raw_value == "<=") {
                return a <= b;
            }
            else if (raw_value == ">") {
                return a > b;
            }
            else if (raw_value == ">=") {
                return a >= b;
            }
        }
    }
    return 0.0;
}

// done
void Comparison::print() {
    cout << "(";
    this->left->print();
    cout << " " << this->raw_value << " ";
    this->right->print();
    cout << ")";
}

variant<bool, double> Equality::get_value() {
    if (left && right) {
        if ((left->data_type != right->data_type)) {
            print_invalid_type();
            // variable update needed here
            return 0.0;
        }
        if (raw_value == "==") {
            return left->raw_value == right->raw_value;
        }
        else if (raw_value == "!=") {
            return left->raw_value != right->raw_value;
        }
    }
    return 0.0;
}

// done
void Equality::print() {
    cout << "(";
    this->left->print();
    cout << " " << this->raw_value << " ";
    this->right->print();
    cout << ")";
}

variant<bool, double> Logical::get_value() {
    if (left && right) {
        if ((left->data_type != BOOL) || (right->data_type != BOOL)) {
            print_invalid_type();
            // variable update needed here
            return 0.0;
        }
        if (holds_alternative<bool>(left->get_value()) && holds_alternative<bool>(right->get_value())) {
            bool a = get<bool>(left->get_value());
            bool b = get<bool>(right->get_value());
            if (raw_value == "&") {
                if (a && b) {
                    return true;
                }
                return false;
            }
            else if (raw_value == "^") {
                if ((a && !b) || (!a && b)) {
                    return true;
                }
                return false;
            }
            else if (raw_value == "|") {
                return a || b; 
            }
        }
    }
    return 0.0;
}

//done
void Logical::print() {
    cout << "(";
    this->left->print();
    cout << " " << this->raw_value << " ";
    this->right->print();
    cout << ")";
}

variant<bool,double> Bool::get_value() {
    return bool_val;
}

void Bool::print() {
    cout << raw_value;
}