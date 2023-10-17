#include "Parser.h"
#include "AST.h"
#include "Node.h"
#include "Operator.h"
#include "Number.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cstdlib>

using namespace std;

Parser::Parser() {
    root = nullptr;
}

void Parser::print_error_2(Token* error_token) const {
    cout << "Unexpected token at line " << error_token->row << " column " << error_token->column 
            << ": " << error_token->value << endl;
    exit(2);
}

void Parser::read_tokens(vector<Token*> tokens_list) {
    Token* current_token = nullptr;
    string current_value;
    set<string> is_operator = {"+", "-", "*", "/"};
    Node* new_node = root;
    Node* curr_node = nullptr;
    double check_double = 0;

    current_token = (tokens_list.at(tokens_list.size() - 1));
    if (current_token->value != "END") {
        cout << "Unexpected token at line " << current_token->row << " column " << current_token->column << ": "
            << current_token->value << endl; 
    }

    for (unsigned int i = 0; i < tokens_list.size(); ++i) {
        current_token = tokens_list.at(i);
        current_value = current_token->value;
        if (is_operator.find(current_value) != is_operator.end()) {
            if (i - 1 < 0) {
                print_error_2(current_token);
            }
            if (tokens_list.at(i - 1) != "(" || i > tokens_list.size() - 2) {
                print_error_2(current_token);
            }
            new_node = new Operator(curr_node, current_node);
            if (root == nullptr) {
                root = new_operator;
            }
            curr_node->add_child(new_node);
            curr_node = new_node; 
        }
        else if (current_value == ")") {
            if (curr_node == nullptr) {
                print_error_2(current_token);
            }
            if ((curr_node->children).size() < 2) {
                print_error_2(current_token);
            }
            curr_node = curr_node->switch_to_parent();
        }
        else if (current_value == "(" ) {
            if (i != 0) {
                if (tokens_list.at(i - 1) == "(") {
                    print_error_2(current_token);
                }
            }
        }
        else {
            new_node = new Number(curr_node, current_token);
            curr_node->add_child(new_node);
        }
    }
}

double Parser::calculate() const {
    if (root == nullptr) {
        return 0;
    }
    Node* current_node = root;
    return calculate_help(current_node);
}

double Parser::calculate_help(Node* operator_node) const {
    // check if the node is operator, if not, return the value;
    if (!operator_node->node_type()) {
        return operator_node->get_number();
    }


    // go through the children nodes
    string operator_sign = operator_node->check_operator();
    vector<Node*>& list_children = operator_node->children;
    double division_check = 0;

    double result = 0;
    Node* current_node = nullptr;
    for (unsigned int i = 0; i < list_children.size(); ++i) {
        current_node = list_children.at(i);
        if (operator_sign == "+") {
            result += calculate_help(current_node);
        }
        else if (operator_sign == "-") {
            result -= calculate_help(current_node);
        }
        else if (operator_sign == "*") {
            result *= calculate_help(current_node);
        }
        else if (operator_sign == "/") {
            division_check = calculate_help(current_node);
            if (i != 0 && division_check == 0) {
                cout << "Runtime error: division by zero." << endl;
                exit(3);
            }
            else {
                result /= division_check;
            }
        }
    }
    return result;
}


void Parser::print() const {
    if (!root) {
        return;
    }
    cout << print_help(root) << endl;
    cout << calculate() << endl;
}

string Parser::print_help(Node* in_node) const {
    if (!in_node->node_type()) {
        return to_string(in_node->get_number());
    }

    string print_expression;
    string expression;
    print_expression += "(";
    expression = in_node->check_operator();

    vector<Node*>& list_children = in_node->children;

    for (unsigned int i = 0; i < list_children.size(); ++i) {
        print_expression += print_help(list_children.at(i));
        if (i != list_children.size() - 1) {
            print_expression += ((list_children.at(i))->check_operator());

        }
    }
    print_expression += ")";
    return print_expression;
}

Parser::~Parser() {

    delete_help(root);
}

void Parser::delete_help(Node* current_node) {
    if (!current_node->node_type()) {
        delete current_node;

        return;
    }

    vector<Node*>& list_children = current_node->children;
    for (unsigned int i = 0; i < list_children.size(); ++i) {
        delete_help(list_children.at(i));
    }
    delete current_node;
}


