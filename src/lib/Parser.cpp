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
    // operator look up
    set<string> is_operator = { "+", "-", "*", "/"};
    bool left = false;
    bool last_operator = false;
    Node* curr_node = root;
    int num_left_parenthesis = 0;
    Node* create_operator = nullptr;
    Node* create_number = nullptr;

    // read each token and make each of them as a node for AST
    for (unsigned int i = 0; i < tokens_list.size(); ++i) {
        // verify the token type
        if (tokens_list.at(i)->value == "(") {
            if (left) {
                print_error_2(tokens_list.at(i));
            }
            num_left_parenthesis += 1;
            left = true;
        }
        else if (tokens_list.at(i)->value == ")") {
            if (left || last_operator) {
                print_error_2(tokens_list.at(i));
            }
            num_left_parenthesis -= 1;
            if (num_left_parenthesis + 1 < 1) {
                print_error_2(tokens_list.at(i));
            }
            curr_node = curr_node->switch_to_parent();
        }
        else if (is_operator.find((tokens_list.at(i))->value) != is_operator.end()) {
            // operator token
            if (!left) {
                print_error_2(tokens_list.at(i));
            }
            left = false;
            create_operator = new Operator(curr_node, tokens_list.at(i));
            if (curr_node == root) {
                root = create_operator;
            }
            else {
                curr_node->add_child(create_operator);
            }
            curr_node = create_operator;
        }
        else {
            // number or END token
            if (i < tokens_list.size() - 1) {
                if (left || num_left_parenthesis == 0) {
                    print_error_2(tokens_list.at(i));
                }
                if (i == token_list.size() - 1) {
                    continue;
                }
                create_number = new Number(curr_node, tokens_list.at(i));
                curr_node->add_child(create_number);
            }
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


