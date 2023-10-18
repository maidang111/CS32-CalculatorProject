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
    string val;
    set<string> operators = { "+", "-" , "*", "/"};
    Node* curr = nullptr;
    Node* create = nullptr;
    bool num_single = false; 
    int num_parenthesis = 0;
    bool last_parenthesis = false;

    for (unsigned i = 0; i < tokens_list.size(); ++i) {
        if (num_parenthesis < 0) {
            print_error_2(tokens_list.at(i));
        }
        val = (tokens_list.at(i))->value;
        if (val == "END") {
            if (num_parenthesis != 0) {
                print_error_2(tokens_list.at(i));
            }
            return;
        }
        if (operators.find(val) != operators.end()) {
            if (num_single) {
                print_error_2(tokens_list.at(i));
            }
            if (i != 0) {
                if ((tokens_list.at(i - 1))->value != "(") {
                    print_error_2(tokens_list.at(i));
                }
            }
            create = new Node(curr, tokens_list.at(i), true);
            if (!root) {
                root = create;
                curr = root;
            }
            else {
                curr->add_child(create);
                curr = create;
            }
        }
        else if (val == "(") {
            if (num_single) {
                print_error_2(tokens_list.at(i));
            }
            num_parenthesis += 1;
            last_parenthesis = true;

        }
        else if (val == ")") {
            if (num_single) {
                num_single = false;
            }
            if (last_parenthesis) {
                print_error_2(tokens_list.at(i));
            }
            num_parenthesis -= 1;
            
        }
        else {
            if (num_single) {
                print_error_2(tokens_list.at(i));
            }
            create = new Node(curr, tokens_list.at(i), false);
            if (last_parenthesis) {
                num_single = true;
            }
            if (!root) {
                root = create;
                curr = root;
            }
            else {
                curr->add_child(create);
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




