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
    // operator list
    set<string> operator_list = {"+", "-", "*", "/"};
    bool last_left_parenthesis = false;
    int number_of_left_parenthesis = 0;
    bool only_number = false;
    Node* curr_operator = root;
    Node* new_node = nullptr;
    bool is_operator = false;

    // go through the vector and convert them into AST
    for (unsigned i = 0; i < tokens_list.size(); ++i) {
        // in the case of ( or ) or END, it does not create node 
        // in the case of (, previous token cannot be (
        if ((tokens_list.at(i))->value == "(") {
            if (last_left_parenthesis || only_number) {
                print_error_2(tokens_list.at(i));
            }
            last_left_parenthesis = true;
            number_of_left_parenthesis += 1;
            is_operator = false;
        }
        // if the last token was (, error 
        else if ((tokens_list.at(i))->value == ")"){
            if (last_left_parenthesis) {
                print_error_2(tokens_list.at(i));
            }
            if (!only_number) {
                if ((curr_operator->children).size() >= 2) {
                    print_error_2(tokens_list.at(i));
                }
                // move operator to its parent node
                curr_operator = curr_operator->switch_to_parent();
            }
            else {
                only_number = false;
            }
            number_of_left_parenthesis -= 1;
            if (number_of_left_parenthesis < 1) {
                print_error_2(tokens_list.at(i));
            }
            last_left_parenthesis = false;
            is_operator = false;
        }
        else if ((tokens_list.at(i))->value == "END") {
            if (number_of_left_parenthesis != 0 || is_operator) {
                print_error_2(tokens_list.at(i));
            }
            return;
        }
        // in case of operator and number token it create node
        else {
            if (number_of_left_parenthesis < 0) {
                print_error_2(tokens_list.at(i));
            }
            if (only_number) {
                print_error_2(tokens_list.at(i));
            }
            // in case of the operator 
            // create the node with the child node
            if (operator_list.find(tokens_list.at(i)->value) != operator_list.end()) {
                if (is_operator) {
                    print_error_2(tokens_list.at(i));
                }
                new_node = new Node(curr_operator, tokens_list.at(i), true);
                if (!root) {
                    root = new_node;
                }
                else {
                    curr_operator->add_child(new_node);

                }
                curr_operator = new_node;
                is_operator = true;
            }
            // in case of number
            // just add new node as the child of the pointer
            else {
                is_operator = false;
                if (last_left_parenthesis) {
                    only_number = true;
                }
                new_node = new Node(curr_operator, tokens_list.at(i), false);
                if (!root) {
                    root = new_node;
                    curr_operator = new_node; 
                }
                else {
                    curr_operator->add_child(new_node);
                }

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
    print_help(root);
    cout << endl << calculate() << endl;
}

void Parser::print_help(Node* in_node) const {
    if (!in_node->node_type()) {
        cout << in_node->get_number();
    }

    string expression;
    cout << "(";
    expression = in_node->return_operator();

    vector<Node*>& list_children = in_node->children;

    for (unsigned int i = 0; i < list_children.size(); ++i) {
        print_help(list_children.at(i));
        if (i != list_children.size() - 1) {
            cout << (list_children.at(i))->return_operator();

        }
    }
    cout << ")";
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


