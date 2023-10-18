#include "Parser.h"
#include "AST.h"
#include "Node.h"
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
    if (tokens_list.size() == 1) {
        return;
    }

    // operator list
    set<string> operator_list = {"+", "-", "*", "/"};
    bool last_left_parenthesis = false;
    int number_parenthesis = 0;
    bool only_number = false;
    Node* curr_node = root;
    Node* new_node = nullptr;
    bool is_operator = false;
    bool is_zero = false;
    int num_operator = 0;

    // go through the vector and convert them into AST
    for (unsigned i = 0; i < tokens_list.size(); ++i) {
        // first case
        // not operator, so number
        if ((tokens_list.at(i))->value == "(") {
            // parenthesis 
            number_parenthesis += 1;
            //in case of (number)
            last_left_parenthesis = true;
            if (only_number || is_zero) {
                print_error_2(tokens_list.at(i));
            }

        } 
        else if ((tokens_list.at(i))->value == ")") {
            if (number_parenthesis <= 0) {
                print_error_2(tokens_list.at(i));
            }
            if (only_number) {
                only_number = false;
            }

            // case * 1 or *
            if ((curr_node->children).size() >= 2) {
                num_operator -= 1;
            }
            if (num_operator < 0) {
                print_error_2(tokens_list.at(i));
            }
            last_left_parenthesis = false;
            number_parenthesis -= 1;
            if (number_parenthesis == 0) {
                is_zero = true;
            }
        }
        else if ((tokens_list.at(i))->value == "END") {
            if (number_parenthesis != 0) {
                print_error_2(tokens_list.at(i));
            }
            return;
        }
        // oeprator
        else if (operator_list.find(((tokens_list.at(i))->value)) != operator_list.end()) {
            num_operator += 1;
            if (only_number || is_zero) {
                print_error_2(tokens_list.at(i));
            }
            new_node = new Node(curr_node, tokens_list.at(i), true);
            if (!root) {
                root = new_node;
            }
            else {
                curr_node->add_child(new_node);
                curr_node = new_node;
            }
            last_left_parenthesis = false;
            // in case of (* 1) or (*)
            is_operator = true;
        } 
        // number
        else {
            if (only_number || is_zero) {
                print_error_2(tokens_list.at(i));
            }
            if (last_left_parenthesis) {
                only_number = true;
            }
            new_node = new Node(curr_node, tokens_list.at(i), false);
            if (!root) {
                root = new_node;
                curr_node = root;
            }
            else {
                curr_node->add_child();
            }
            last_left_parenthesis = false;
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
    string operator_sign = operator_node->return_operator();
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
    print_help(root, false);
    cout << endl;
    cout << calculate();
    cout << endl;
}

void Parser::print_help(Node* in_node, bool parenthesis) const {
    if (!in_node->node_type()) {
        cout << in_node->get_number();
        return;
    }
    string expression;
    if (parenthesis) {
        cout << "(";
    }
    expression = in_node->return_operator();

    vector<Node*>& list_children = in_node->children;

    for (unsigned int i = 0; i < list_children.size(); ++i) {
        print_help(list_children.at(i), true);
        if (i != list_children.size() - 1) {
            cout << (list_children.at(i))->return_operator();

        }
    }
    if (parenthesis) {
        cout << ")";
    }
}

Parser::~Parser() {
    delete_help(root);
}

void Parser::delete_help(Node* current_node) {
    // delete the node if the node is number
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




