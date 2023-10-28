#include "Parser.h"
#include "AST.h"
#include "Lexer.h"
#include "Node.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cstdlib>
#include <stack>
#include <map>

using namespace std;

Parser::Parser() {
}

bool Parser::check_double(string s) const {
    bool dot = false;
    bool num_val = false;
    for (unsigned int i = 0; i < s.size(); ++i) {
        if (!isdigit(s.at(i)) && dot) {
            return false;
        }
        else if (s.at(i) == '.') {
            dot = true;
        }
        else if (isdigit(s.at(i))) {
            num_val = true;
        }
    }
    if (!num_val) {
        return false;
    }
    return true;
}

void Parser::print_error_2(Token* error_token) {
    cout << "Unexpected token at line " << error_token->row << " column " << error_token->column 
            << ": " << error_token->value << endl;
    for (unsigned int i = 0; i < roots.size(); ++i) {
        delete_help(roots.at(i));
    }
    exit(2);

}

void Parser::read_all_lines(Lexer in_lexer) {
    vector<Token*> build_AST;
    vector<Token*>& whole_tokens = in_lexer.tokens;
    if (whole_tokens.size() < 2) {
        print_error_2(whole_tokens.at(0));
    }

    int num_parenthesis = 0;

    size_t i = 0;
    while (i < whole_tokens.size()) {
        build_AST.clear();
        for (size_t j = i; j < whole_tokens.size(); ++j) {
            if (whole_tokens.at(j)->value == "(") {
                num_parenthesis += 1;
            }
            else if (whole_tokens.at(j)->value == ")") {
                num_parenthesis -= 1;
            }
            build_AST.push_back(whole_tokens.at(j));
            if ((num_parenthesis == 0 && j != i) || (j == i && check_double(whole_tokens.at(j)->value))) {
                i = j + 1;
                break;
            }
            else if (j + 1 == whole_tokens.size()) {
                i = whole_tokens.size();

            }
        }
        roots.push_back(read_tokens(build_AST, nullptr));
    }
    if (!roots.empty()) {
        delete_help(roots.at(roots.size() - 1));
        roots.pop_back();
    }
}

Node* Parser::read_tokens(vector<Token*>& tokens_list, Node* new_root) {
    // no expression
    string val;
    set<string> operators = { "+", "-" , "*", "/", "="};
    // int num_left_parenthesis = 0; 
    // int num_parenthesis = 0;
    int num_operator = 0;
    Node* curr = nullptr;
    Node* create = nullptr;
    bool num_single = false; 
    int num_parenthesis = 0;
    bool last_left = false;
    bool first_zero = false;
    // bool was_equal_sign = false;
    // bool single_operand = false;
    // bool is_equal = false;
    int equal_parenthesis = 0;

    // equality
    size_t begin = 0;
    // size_t end = 0;
    vector<size_t> breakpoint; 
    bool inside_single = false;
    bool equal_error = false;
    size_t error_index = 0;
    int m = 0;

    if (tokens_list.at(0)->value == "(") {
        // cout << "b" << endl;
        first_parenthesis.push_back(true);
    }
    else {
        first_parenthesis.push_back(false);
    }
    for (size_t i = 0; i < tokens_list.size(); ++i) {
        val = tokens_list.at(i)->value;
        // cout << val << endl;
        // for testing
        // cout << "check: " << val << endl;
        if (equal_error && i == error_index) {
            // cout << "print_error" << endl;
            print_error_2(tokens_list.at(i));
        }
        if (first_zero && val != "END") {
            // cout << "c" << endl;
            print_error_2(tokens_list.at(i));
        }
        else if (val == "END") {
            if (num_parenthesis != 0) {
                // cout << "d" << endl;
                print_error_2(tokens_list.at(i));
            }
            return new_root;
        }
        if (operators.find(val) != operators.end()) {
            // cout << "Operator: " << val << endl;
            if (num_single) {
                // cout << "e" << endl;
                print_error_2(tokens_list.at(i));
            }
            if (!last_left) {
                // cout << "f" << endl;
                print_error_2(tokens_list.at(i));
            }
            last_left = false;
            // AST
            // testing
            // cout << "Create Operator " << val << endl;
            if (val != "=") {
                create = new Node(curr, tokens_list.at(i), true, false, false);
            }
            else {
                if (i + 1 < tokens_list.size()) {
                    if (check_double(tokens_list.at(i + 1)->value) || tokens_list.at(i + 1)->value == ")") {
                        // cout << "k" << endl;
                            print_error_2(tokens_list.at(i + 1));
                    }
                }
                begin = i + 1;
                // is_equal = true;
                create = new Node(curr, tokens_list.at(i), true, true, false);
                // was_equal_sign = true;
                // equal_parenthesis = num_parenthesis;
                // check if the first operand is variable for = sign and also not ) 

                // check error inside of = 
                m = 0;
                for (size_t w = begin; w < tokens_list.size(); ++w) {
                    if (tokens_list.at(w)->value == "(") {
                        ++m;
                    }
                    else if (tokens_list.at(w)->value == ")") {
                        --m;
                    }
                    if (m == -1) {        
                        break;
                    }
                    if (!inside_single) {
                        // cout << "breakpoint" << w << endl;
                        breakpoint.push_back(w);
                        inside_single = true;
                    }
                    if (m == 0) {
                        inside_single = false;
                    }
                }
                // move to here
                if (m != -1) {
                    delete create;
                    print_error_2(tokens_list.at(tokens_list.size() -1));
                }
                begin = 0;
                equal_parenthesis = num_parenthesis;
                for (size_t w = breakpoint.at(0); w < breakpoint.at(breakpoint.size() - 1); ++w) {
                    if (w == breakpoint.at(begin + 1)) {
                        begin += 1;
                    }
                    if (operators.find(tokens_list.at(w)->value) != operators.end()) {
                        equal_error = true;
                        error_index = breakpoint.at(begin);
                        // change here
                        delete create;
                        // another change
                        if (begin == 0) {
                            print_error_2(tokens_list.at(breakpoint.at(begin)));
                        }
                        print_error_2(tokens_list.at(breakpoint.at(breakpoint.size() - 1)));
                        break;
                    }
                    else if (check_double(tokens_list.at(w)->value)) {
                        equal_error = true;
                        error_index = w;
                        // change here;
                        delete create;
                        print_error_2(tokens_list.at(breakpoint.at(breakpoint.size() - 1)));
                        break;   
                    }
                }
                breakpoint.clear();
            }
            num_operator += 1;
            if (!new_root) {
                curr = create;
                new_root = create;
            }
            else {
                curr->add_child(create);
                curr = create;
            }
        }
        else if (val == ")") {
            if (last_left) {
                // cout << last_left << " " << single_operand << endl;
                // cout << "g" << endl;
                print_error_2(tokens_list.at(i));
            }
            num_parenthesis -= 1;
            if (num_parenthesis < 0) {
                // cout << "h" << endl;
                print_error_2(tokens_list.at(i));
            }
            else if (num_parenthesis == 0) {
                first_zero = true;
            }
            if (curr->data->value == "=" && num_parenthesis + 1 == equal_parenthesis) {
                if (curr->children.size() < 2) {
                    print_error_2(tokens_list.at(i));
                }
            }
            // AST check if the operator has two child
            if (!num_single) {
                // change here
                if ((curr->children).size() < 1) {
                    // cout << "i" << endl;
                    print_error_2(tokens_list.at(i));
                    // cout << "!curr->children.size().at(i)" << endl;
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
        }
        else if (val == "(") {
            if (num_single) {
                cout << "j" << endl;
                print_error_2(tokens_list.at(i));
            }
            last_left = true;
            num_parenthesis += 1;
        }
        else {
            if (num_single) {
                // cout << "k" << endl;
                print_error_2(tokens_list.at(i));
            }
            if (last_left || i == 0) {
                num_single = true;
                if (last_left && tokens_list.at(i + 1)->value != ")") {
                    // cout << "l" << endl;
                    print_error_2(tokens_list.at(i));
                }
            }
            // if (curr->data->value == "=" && single_operand) {
            //     single_operand = false;
            // }
            last_left = false;
            // if (was_equal_sign) {
            //     single_operand = true;
            //     was_equal_sign = false;
            // }
            // else {
            //     single_operand = false;
            // }

            // check if the variable already exists, if not , update it to the varaibles map
            if (!check_double(val)) {
                if (variables.find(val) == variables.end()) {
                    // insert new variables
                    variables.emplace(val, -1.0);
                    known_variables.emplace(val, false);
                }
            }
        
            // AST
            // testing
            // cout << "Creating number: " << val << endl;
            if (!check_double(val)) {
                create = new Node(curr, tokens_list.at(i), false, false, true);
            }
            else {
                create = new Node(curr,tokens_list.at(i), false, false, false);
            }
            if (!new_root) {
                // cout << "1" << endl;
                new_root = create;
                curr = new_root;
            }
            else {
                // cout << "2" << endl;
                curr->add_child(create);
                // cout << "3" << endl;
            }
        }
        // cout << "3-1" << endl;
    }
    return new_root;
}


double Parser::calculate(Node* root) {
    if (root == nullptr) {
        return 0;
    }
    Node* current_node = root;
    return calculate_help(current_node);
}


double Parser::calculate_help(Node* operator_node) {
    // in case of variable
    if (operator_node->is_variable) {
        if (!known_variables.at(operator_node->data->value)) {
            cout << "Runtime error: unknown identifier " << operator_node->data->value << endl;
            for (unsigned int i = 0; i < roots.size(); ++i) {
                delete_help(roots.at(i));
            }
            exit(3);
        }
        else {
            return variables.at(operator_node->data->value);
        }
    }
    // in case of number
    if (!operator_node->node_type()) {
        return operator_node->get_number();
    }
    double v1 = 0;
    if (operator_node->equality_sign) {
        v1 = calculate_help(operator_node->children.at(operator_node->children.size() - 1));
        for (unsigned int i = 0; i + 1 < operator_node->children.size(); ++i) {
            variables.at(operator_node->children.at(i)->data->value) = v1;
            known_variables.at(operator_node->children.at(i)->data->value) = true;
        }
        return v1;
    }

    Node* current_node = nullptr;
    string operator_sign = operator_node->return_operator();
    double division_check = 0;
    double first_child = calculate_help((operator_node->children).at(0));
    vector<Node*>& list_children = operator_node->children;
    if (list_children.size() == 1) {
        return first_child;
    }
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
                for (unsigned int i = 0; i < roots.size(); ++i) {
                    delete_help(roots.at(i));
                }
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
    if (roots.empty()) {
        return;
    }
    for (unsigned int i = 0; i < roots.size(); ++i) {
        print_help(roots.at(i), first_parenthesis.at(i));
        cout << endl << calculate(roots.at(i)) << endl;
    }
}

void Parser::print_help(Node* in_node, bool parenthesis) {
    if (!in_node) {
        return;
    }
    if (in_node->is_variable) {
        cout << in_node->data->value;
        return;
    }
    else if (!in_node->node_type()) {
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
    for (unsigned int i = 0; i < roots.size(); ++i) {
        delete_help(roots.at(i));
    }
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