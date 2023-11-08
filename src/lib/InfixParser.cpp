#include "InfixParser.h"
#include "AST_Node.h"
#include "Lexer.h"
#include <string>
#include <vector>
using namespace std;

InfixParser::InfixParser() {
    Lexer a;
    a.create_endtokens();
    index = 0;
    tokens = a.multi_end_tokens;
    operators = {"+", "-", "*", "/", "%", "&", "|", "^", "<", "<=", ">", ">=", "!=", "==", "="};
    // check tokens
    // cout << "check lexer" << endl;
    // for (auto a: tokens) {
    //     cout << a->raw_value << endl;
    // }
    // cout << "finish checking" << endl;
    error = false;
    result_double = 0;
    result_bool = false;
}

InfixParser::~InfixParser() {
    for (const auto& a: ASTs) {
        delete_help(a);
    }
}

void InfixParser::read_all_token() {
    while (tokens.at(index)->raw_value != "END") {
        // cout << index << endl;
        read_token();
        error_index.clear();
        // cout << index << endl;
    }
    // cout << "read_all_no_error" << endl;
}

void InfixParser::read_token() {
    if (index >= tokens.size()) {
        return;
    }
    size_t curr_index = index;
    // cout << "1 Index here: " << index<< "  currIndex: " << curr_index << endl;
    AST_Node* root = nullptr;
    while (tokens.at(curr_index)->raw_value != "END") {
        ++curr_index;
    }
    // cout << "2 Index here: " << index<< "  currIndex: " << curr_index << endl;
    curr_index -= 1;
    // cout << "index: " << index << endl;
    // cout << "curr_index: " << curr_index << endl;
    // cout << "1 Index here: " << index<< "  currIndex: " << curr_index << endl;
    // cout << "1 Index here: " << index<< "  currIndex: " << curr_index << endl;
    check_parenthesis(index, curr_index);
    check_operator(index, curr_index);
    if (!error_index.empty()) {
        size_t min = error_index.at(0);
        for (size_t j = 0; j < error_index.size(); ++j) {
            if (min > error_index.at(j)) {
                min = error_index.at(j);
            }
        }
        // cout << "unexpected token at line 1 column " << tokens.at(min)->column << ": " << tokens.at(min)->raw_value << endl;
        index = curr_index + 2;
        return;
    }
    // cout << "Here++++++++++++++++++++++++" << endl;

    root = read_one_line(index, curr_index, nullptr);
    if (!error) {
        ASTs.push_back(root);
    }
    else {
        delete_help(root);
    }
    index = curr_index + 2;
    error = false;
    // cout << "ended" << endl;
    // one line 
    // cout << "checking index: " << index << endl;
}

void InfixParser::check_parenthesis(size_t first_element, size_t last_element) {
    // cout << "check_parenthesis: " << endl;
    // cout << first_element << " " << last_element << endl;
    // if (first_element >= last_element -1) {
    //     return;
    // }
    size_t first_error = 0;
    int count = 0;
    size_t last_left = 0;
    bool is_error = false;
    
    for (size_t i = first_element; i < last_element + 2; ++i) {
        // cout << "index: " << i << endl;
        if (tokens.at(i)->raw_value == "(") {
            count++;
            last_left = i;
            // cout << 10 << endl;
        }
        else if (tokens.at(i)->raw_value == ")") {
            count--;
            if (last_left + 1 == i) {
                first_error = i;
                is_error = true;
                break;
                // cout << 11 << endl;
            }
        }
        if (count < 0) {
            first_error = i;
            is_error = true;
            // cout << 12 << endl;
            break;
        }
    }
    if ((count != 0) && !is_error) {
        is_error = true;
        first_error = last_element + 1;
        // cout << 13 << endl;
    }
    // cout << "is_error: " << is_error << " first_error: " << first_error << endl;
    if (is_error) {
        error_index.push_back(first_error);
    }
}

void InfixParser::check_operator(size_t first_element, size_t last_element) {
    // if (first_element + 1 >= last_element) {
    //     return;
    // }
    bool is_error = false;
    size_t first_error = 0;

    for (size_t i = first_element; i < last_element + 2; ++i) {
        if (operators.count(tokens.at(i)->raw_value)) {
            if ((i == first_element) || (i == last_element)) {
                is_error = true;
                first_error = i;
                break;
            }
            if (operators.count(tokens.at(i - 1)->raw_value) || (tokens.at(i - 1)->raw_value == "(")) {
                is_error = true;
                first_error = i;
                break;
            }
            if (tokens.at(i + 1)->raw_value == ")") {
                is_error = true;
                first_error = i;
                break;
            }
        }
    }
    if (is_error) {
        error_index.push_back(first_error);
    }

}




AST_Node* InfixParser::read_one_line(size_t begin_line, size_t end_line, AST_Node* in_parent) {
    // cout << "enter" << endl;
    if (begin_line > end_line) {
        // cout << "here" << endl;
        // error here for parenthesis () 
        return nullptr;
    }
    if (begin_line == end_line) {
        // direct values such as numbers, true/false, variables
        if (isalpha(tokens.at(begin_line)->raw_value.at(0)) && (tokens.at(begin_line)->raw_value != "true") && (tokens.at(begin_line)->raw_value != "false")) {
            //variable
            Variable_Val* add_variable = new Variable_Val(tokens.at(begin_line));
            add_variable->single_val = true;
            return add_variable;
        }
        else if (isdigit((tokens.at(begin_line)->raw_value).at(0)) || (tokens.at(begin_line)->raw_value == "true") || (tokens.at(begin_line)->raw_value == "false")) {
            Direct_Val* direct_val = new Direct_Val(tokens.at(begin_line));
            direct_val->single_val = true;
            return direct_val;
        }
        // error not numbers, true/false, or variables
        return nullptr;
    }
    int count = 0;
    for (size_t i = begin_line; i <= end_line; ++i) {
        // cout << "first: " << i << endl;
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (tokens.at(i)->raw_value == "=")) {
            Assign* add = new Assign(tokens.at(i));
            add->parent = in_parent;
            add->left = read_one_line(begin_line, i - 1, add);
            add->right = read_one_line(i + 1, end_line, add);
            // only one side has 
            if (!add->left || !add->right) {
                delete_help(add);
            }
            return add;
        }
    }
    // cout << "poitnt 1" << endl;

    count = 0;
    // cout << "end_line: " << end_line << endl;
    // cout << "begin_line: " << begin_line << endl;
    Boolean_Operation* add_bool = nullptr;
    for (size_t i = end_line; i >= begin_line; --i) {
        // cout << "loop top: " << i << endl;
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (tokens.at(i)->raw_value == "|")) {
            add_bool = new Boolean_Operation(tokens.at(i));
            add_bool->parent = in_parent;
            add_bool->left = read_one_line(begin_line, i - 1, add_bool);
            add_bool->right = read_one_line(i + 1, end_line, add_bool);
            if (!add_bool->left || !add_bool->right) {
                delete_help(add_bool);
            }
            return add_bool;
        }
        if (i == 0) {
            break;
        }
        // cout << "loop bottom: " << i << endl;
    }
    // cout << "point2" << endl;
    count = 0;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (tokens.at(i)->raw_value == "^")) {
            add_bool = new Boolean_Operation(tokens.at(i));
            add_bool->parent = in_parent;
            add_bool->left = read_one_line(begin_line, i - 1, add_bool);
            add_bool->right = read_one_line(i + 1, end_line, add_bool);
            if (!add_bool->left || !add_bool->right) {
                delete_help(add_bool);
            }
            return add_bool;
        }
        if (i == 0) {
            break;
        }
    }

    count = 0;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (tokens.at(i)->raw_value == "&")) {
            add_bool = new Boolean_Operation(tokens.at(i));
            add_bool->parent = in_parent;
            add_bool->left = read_one_line(begin_line, i - 1, add_bool);
            add_bool->right = read_one_line(i + 1, end_line, add_bool);
            if (!add_bool->left || !add_bool->right) {
                delete_help(add_bool);
            }
            return add_bool;
        }
        if (i == 0) {
            break;
        }
    }

    count = 0;
    Equality_Val* new_equal = nullptr;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (tokens.at(i)->raw_value == "!=")) {
            new_equal = new Equality_Val(tokens.at(i));
            new_equal->parent = in_parent;
            new_equal->left = read_one_line(begin_line, i - 1, new_equal);
            new_equal->right = read_one_line(i + 1, end_line, new_equal);
            if (!new_equal->left || !new_equal->right) {
                delete_help(new_equal);
            }
            return new_equal;
        }
        else if ((count == 0) && (tokens.at(i)->raw_value == "==")) {
            new_equal = new Equality_Val(tokens.at(i));
            new_equal->parent = in_parent;
            new_equal->left = read_one_line(begin_line, i - 1, new_equal);
            new_equal->right = read_one_line(i + 1, end_line, new_equal);
            if (!new_equal->left || !new_equal->right) {
                delete_help(new_equal);
            }        
            return new_equal;
        }
        if (i == 0) {
            break;
        }
    }

    count = 0;
    Comparison_Val* new_compare = nullptr;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (tokens.at(i)->raw_value == ">=")) {
            new_compare = new Comparison_Val(tokens.at(i));
            new_compare->parent = in_parent;
            new_compare->left = read_one_line(begin_line, i - 1, new_compare);
            new_compare->right = read_one_line(i + 1, end_line, new_compare);
            if (!new_compare->left || !new_compare->right) {
                delete_help(new_compare);
            }
            return new_compare;
        }
        else if ((count == 0) && (tokens.at(i)->raw_value == ">")) {
            new_compare = new Comparison_Val(tokens.at(i));
            new_compare->parent = in_parent;
            new_compare->left = read_one_line(begin_line, i - 1, new_compare);
            new_compare->right = read_one_line(i + 1, end_line, new_compare);
            if (!new_compare->left || !new_compare->right) {
                delete_help(new_compare);
            }
            return new_compare;
        }
        else if ((count == 0) && (tokens.at(i)->raw_value == "<=")) {
            new_compare = new Comparison_Val(tokens.at(i));
            new_compare->parent = in_parent;
            new_compare->left = read_one_line(begin_line, i - 1, new_compare);
            new_compare->right = read_one_line(i + 1, end_line, new_compare);
            if (!new_compare->left || !new_compare->right) {
                delete_help(new_compare);
            }
            return new_compare;
        } 
        else if ((count == 0) && (tokens.at(i)->raw_value == "<")) {
            new_compare = new Comparison_Val(tokens.at(i));
            new_compare->parent = in_parent;
            new_compare->left = read_one_line(begin_line, i - 1, new_compare);
            new_compare->right = read_one_line(i + 1, end_line, new_compare);
            if (!new_compare->left || !new_compare->right) {
                delete_help(new_compare);
            }    
            return new_compare;
        }
        if (i == 0) {
            break;
        }
    }

    count = 0;
    Double_Operation* double_add = nullptr;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (tokens.at(i)->raw_value == "-")) {
            double_add = new Double_Operation(tokens.at(i));
            double_add->parent = in_parent;
            double_add->left = read_one_line(begin_line, i - 1, double_add);
            double_add->right = read_one_line(i + 1, end_line, double_add);
            if (!double_add->left || !double_add->right) {
                delete_help(double_add);
            }
            return double_add;
        }
        else if ((count == 0) && (tokens.at(i)->raw_value == "+")) {
            // cout << "+" << endl;
            double_add = new Double_Operation(tokens.at(i));
            double_add->parent = in_parent;
            double_add->left = read_one_line(begin_line, i - 1, double_add);
            double_add->right = read_one_line(i + 1, end_line, double_add);
            if (!double_add->left || !double_add->right) {
                delete_help(double_add);
            }
            return double_add;
        }
        if (i == 0) {
            break;
        }
    }

    count = 0;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (tokens.at(i)->raw_value == "%")) {
            double_add = new Double_Operation(tokens.at(i));
            double_add->parent = in_parent;
            double_add->left = read_one_line(begin_line, i - 1, double_add);
            double_add->right = read_one_line(i + 1, end_line, double_add);
            if (!double_add->left || !double_add->right) {
                delete_help(double_add);
            }
            return double_add;
        }
        else if ((count == 0) && (tokens.at(i)->raw_value == "/")) {
            double_add = new Double_Operation(tokens.at(i));
            double_add->parent = in_parent;
            double_add->left = read_one_line(begin_line, i - 1, double_add);
            double_add->right = read_one_line(i + 1, end_line, double_add);
            if (!double_add->left || !double_add->right) {
                delete_help(double_add);
            }
            return double_add;
        }
        else if ((count == 0) && (tokens.at(i)->raw_value == "*")) {
            double_add = new Double_Operation(tokens.at(i));
            double_add->parent = in_parent;
            double_add->left = read_one_line(begin_line, i - 1, double_add);
            double_add->right = read_one_line(i + 1, end_line, double_add);
            if (!double_add->left || !double_add->right) {
                delete_help(double_add);
            }
            return double_add;
        }
        if (i == 0) {
            break;
        }
    }
    return read_one_line(begin_line + 1, end_line - 1, in_parent);
}

void InfixParser::delete_help(AST_Node* in_node) {
    if (!in_node) {
        return;
    }
    delete_help(in_node->left);
    delete_help(in_node->right);
    delete in_node;
}

void InfixParser::print_all() {
    // cout << "print all: enter" << endl;
    for (size_t i = 0; i < ASTs.size(); ++i) {
        // cout << "print all: loop top\tindex: " << i << endl;
        print_AST(ASTs.at(i));
        cout << endl;
        // cout << "print all:loop middle\tindex: " << i << endl;
        evaluate_print(ASTs.at(i));
        // cout << "print all:loop bottom\tindex: " << i << endl;
    }
    // cout << "print all: end" << endl;
}
void InfixParser::print_AST(AST_Node* node) const {
    if (!node) {
        return;
    }
    if (!node->single_val) {
        cout << "(";
    }
    print_AST(node->left);
    if (!node->single_val) {
        cout << " ";
    }
    cout << node->data->raw_value;
    if (!node->single_val) {
        cout << " ";
    }
    print_AST(node->right);
    if (!node->single_val) {
        cout << ")";
    }
    return;
}

Data InfixParser::evaluate(AST_Node* in_node) {
    if (!in_node) {
        Data a;
        return a;
    }
    Data left_val = evaluate(in_node->left);
    // cout << "left: " << left_val.double_val << endl;
    Data right_val = evaluate(in_node->right);
    // cout << "right: " << right_val.double_val << endl;
    return in_node->get_value(left_val, right_val);
}

void InfixParser::evaluate_print(AST_Node* head) {
    Data calculate = evaluate(head);
    // cout << "calculation ended ++++++++++++++++" << endl;
    // for (auto b: Data::curr_variables) {
    //     cout << b.first << " " << endl;
    // }
    // cout << "+++++++++++++=" << endl;
    if (calculate.data_type == "DOUBLE") {
        cout << calculate.double_val << endl;
        update_variables();
    }
    else if (calculate.data_type == "BOOL") {
        if (calculate.bool_val) {
            cout << "true" << endl;
        }
        else if (!calculate.bool_val) {
            cout << "false" << endl;
        }
        update_variables();
    }
    AST_Node::runtime_error = false;
}

void InfixParser::update_variables() {
    // cout << "before update: " << endl;
    // for (auto b: Data::curr_variables) {
    //     cout << b.first << endl;
    // }
    // cout << "complete before update " << endl;
    for (auto a: Data::curr_variables) {
        if (AST_Node::prev_variables.find(a.first) != AST_Node::prev_variables.end()) {
            AST_Node::prev_variables.at(a.first) = a.second;
        }
        else  {
            AST_Node::prev_variables.emplace(a.first, a.second);
        }
    }
    // cout << "after update: " << endl;
    // for (auto c: AST_Node::prev_variables) {
    //     cout << c.first << endl;
    // }
    // cout << "complete after update:" << endl;
}