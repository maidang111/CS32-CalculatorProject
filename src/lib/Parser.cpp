#include "Parser.h"
#include "AST.h"
#include "Node.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cstdlib>
#include <stack>

using namespace std;

Parser::Parser() {
    root = nullptr;
    first_parenthesis = false;
}

void Parser::print_error_2(Token* error_token) {
    cout << "Unexpected token at line " << error_token->row << " column " << error_token->column 
            << ": " << error_token->value << endl;
    delete_help(root);
    exit(2);

}


void Parser::read_tokens(vector<Token*> tokens_list) {
    // no expression
    if (tokens_list.size() == 1) {
        print_error_2(tokens_list.at(0));
    }
    string val;
    set<string> operators = { "+", "-" , "*", "/"};
    // int num_left_parenthesis = 0; 
    // int num_parenthesis = 0;
    int num_operator = 0;
    Node* curr = nullptr;
    Node* create = nullptr;
    bool num_single = false; 
    int num_parenthesis = 0;
    bool last_left = false;
    bool first_zero = false;
    stack<string> operator_list;
    if (tokens_list.at(0)->value == "(") {
        first_parenthesis = true;
    }
    for (unsigned i = 0; i < tokens_list.size(); ++i) {
        val = tokens_list.at(i)->value;
        // for testing
        // cout << "check: " << val << endl;
        // cout << i << endl;
        if (first_zero && val != "END") {
            cout << "first zero" << endl;
            print_error_2(tokens_list.at(i));
        }
        else if (val == "END") {
            cout << "END" << endl;
            if (num_parenthesis != 0) {
                // cout << "1" << endl;
                print_error_2(tokens_list.at(i));
            }
            return;
        }
        if (operators.find(val) != operators.end()) {
            operator_list.push(val);
            // cout << "Operator: " << val << endl;
            if (num_single) {
                // cout << "2" << endl;
                print_error_2(tokens_list.at(i));
            }
            if (!last_left) {
                // cout << "3" << endl;
                print_error_2(tokens_list.at(i));
            }
            last_left = false;
            // AST
            // testing
            // cout << "Create Operator " << val << endl;
            create = new Node(curr, tokens_list.at(i), true);
            num_operator += 1;
            if (!root) {
                curr = create;
                root = create;
            }
            else {
                curr->add_child(create);
                curr = create;
            }
        }
        else if (val == ")") {
            if (last_left) {
                // cout << "last_left" << endl;
                print_error_2(tokens_list.at(i));
            }
            num_parenthesis -= 1;
            if (num_parenthesis < 0) {
                // cout << "num_parenthesis < 0" << endl;                
                print_error_2(tokens_list.at(i));
            }
            else if (num_parenthesis == 0) {
                first_zero = true;
            }
            // AST check if the operator has two child
            if (!num_single) {
                if ((curr->children).size() < 2 && operator_list.top() != "/" && operator_list.top() != "*") {
                    cout << "!curr->children.size().at(i)" << endl;
                    print_error_2(tokens_list.at(i));
                }
                else {
                    num_operator -= 1;
                    // cout << "Go to parent" << endl;
                    if (num_operator >= 1) {
                        curr = curr->switch_to_parent();
                    }
                    // cout << "Seg fault for going to parent\n";
                }
            }
            else {
                num_single = false;
            }
            operator_list.pop();
        }
        else if (val == "(") {
            if (num_single) {
                // cout << "3" << endl;
                print_error_2(tokens_list.at(i));
            }
            last_left = true;
            num_parenthesis += 1;
        }
        else {
            if (num_single) {
                // cout << "5" << endl;
                print_error_2(tokens_list.at(i));
            }
            if (last_left || i == 0) {
                num_single = true;
                if (last_left && tokens_list.at(i + 1)->value != ")") {
                    // cout << "7" << endl;
                    print_error_2(tokens_list.at(i));
                }
            }
            last_left = false;
            // AST
            // testing
            // cout << "Creating number: " << val << endl;
            create = new Node(curr, tokens_list.at(i), false);
            if (!root) {
                // cout << "1" << endl;
                root = create;
                curr = root;
            }
            else {
                // cout << "2" << endl;
                curr->add_child(create);
                // cout << "3" << endl;
            }
        }
    }
}



double Parser::calculate() {
    if (root == nullptr) {
        return 0;
    }
    Node* current_node = root;
    return calculate_help(current_node);
}

double Parser::calculate_help(Node* operator_node) {
    // check if the node is operator, if not, return the value;
    if (!operator_node->node_type()) {
        return operator_node->get_number();
    }


    // go through the children nodes
    Node* current_node = nullptr;
    string operator_sign = operator_node->return_operator();
    double division_check = 0;
    double first_child = calculate_help((operator_node->children).at(0));
    vector<Node*>& list_children = operator_node->children;
    for (unsigned int i = 1; i < list_children.size(); ++i) {
        current_node = list_children.at(i);
        if (operator_sign == "+") {
            first_child += calculate_help(current_node);
        }
        else if (operator_sign == "-") {
            first_child -= calculate_help(current_node);
        }
        else if (operator_sign == "*") {
            first_child *= calculate_help(current_node);
        }
        else if (operator_sign == "/") {
            division_check = calculate_help(current_node);
            if (i != 0 && division_check == 0) {
                cout << "Runtime error: division by zero." << endl;
                delete_help(root);
                exit(3);
            }
            else {
                first_child /= division_check;
            }
        }
    }
    return first_child;
}


void Parser::print() {
    if (!root) {
        return;
    }
    print_help(root, first_parenthesis);
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
            cout << " " << expression << " ";
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
    if (!current_node) {
        return;
    }
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