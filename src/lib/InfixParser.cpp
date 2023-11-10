#include "InfixParser.h"
#include "AST_Node.h"
#include "Lexer.h"
#include <cctype>
#include <string>
#include <vector>
using namespace std;

//creates infixParser using a list of tokens from lexer
InfixParser::InfixParser(vector<Token*>& tokens) {
    index = 0;
    this->tokens = tokens;
    operators = {"+", "-", "*", "/", "%", "&", "|", "^", "<", "<=", ">", ">=", "!=", "==", "="};
    error = false;
    result_double = 0;
    result_bool = false;
}
InfixParser::InfixParser(){
    index = 0;
    operators = {"+", "-", "*", "/", "%", "&", "|", "^", "<", "<=", ">", ">=", "!=", "==", "="};

    error = false;
    result_double = 0;
    result_bool = false;
}
InfixParser::~InfixParser() {
    for (const auto& a: ASTs) {
        delete_help(a);
    }
}

// void InfixParser::testing_AST() {
//     cout << "testing ast" << endl;
//     for (size_t i = 0; i < ASTs.size(); ++i) {
//         if (ASTs.at(i) == nullptr) {
//             cout << i << " is nullptr" << endl;
//         }
//         else {
//             cout << "AST head val: " << ASTs.at(i)->data->raw_value << endl;
//         }
//     }
//     cout << "testing finished" << endl;
// }

void InfixParser::read_all_token(bool calc) {
    if (tokens.size() <= 1) {
        return;
    }
    // cout << "tokens size() " << tokens.size() << endl;
    while (index < tokens.size()) {
        // cout << index << endl;
        // cout << "index: " << index << endl;
        read_token(calc);
        error_index.clear();
        // cout << "ast size(): " << ASTs.size() << endl;
        // cout << index << endl;
    }
    if (!ASTs.at(ASTs.size() - 1)) {
        ASTs.pop_back();
    }
    // testing_AST();
    // cout << "read_all_no_error" << endl;
}

AST_Node* InfixParser::single_value_token(size_t begin_a) {
    if (tokens.at(begin_a)->raw_value == ")") {
        cout << "Unexpected token at line 1 column " << tokens.at(begin_a)->column << ": " << tokens.at(begin_a)->raw_value << endl;
        return nullptr;
    }
    else if (tokens.at(begin_a)->raw_value == "(") {
        cout << "Unexpected token at line 1 column " << tokens.at(begin_a + 1)->column << ": " << tokens.at(begin_a + 1)->raw_value << endl;
        return nullptr;
    }
    else if (operators.count(tokens.at(begin_a)->raw_value)) {
        cout << "Unexpected token at line 1 column " << tokens.at(begin_a + 1)->column << ": " << tokens.at(begin_a)->raw_value << endl;
        return nullptr;
    }
    else if (((isalpha(tokens.at(begin_a)->raw_value.at(0)) || (tokens.at(begin_a)->raw_value.at(0) == '_'))
                && ((tokens.at(begin_a)->raw_value != "true") && (tokens.at(begin_a)->raw_value) != "false"))) {
        Variable_Val* new_val = new Variable_Val(tokens.at(begin_a));
        new_val->single_val = true;
        return new_val;
    }
    else {
        Direct_Val* new_val = new Direct_Val(tokens.at(begin_a));
        new_val->single_val = true;
        return new_val;
    }
}

bool InfixParser::check_for_statement(size_t begin_line, size_t end_line) const {
    for (size_t j = begin_line; j <= end_line; ++j) {
        if (tokens.at(j)->raw_value == "print") {
            cout << "Unexpected token at line 1 column " << tokens.at(j)->column << ": " << tokens.at(j)->raw_value << endl;
            return true;
        }
        else if (tokens.at(j)->raw_value == "if") {
            cout << "Unexpected token at line 1 column " << tokens.at(j)->column << ": " << tokens.at(j)->raw_value << endl;
            return true;
        }
    }
    return false;
}

void InfixParser::read_token(bool calc) {
    if (index >= tokens.size()) {
        // cout << "return read_token" << endl;
        return;
    }
    size_t curr_index = index;
    AST_Node* root = nullptr;
    while (tokens.at(curr_index)->raw_value != "END") {
        ++curr_index;
    }

    curr_index -= 1;
    if (calc) {
        if (check_for_statement(index, curr_index)) {
            index = curr_index + 2;
            return;
        }
    }
    if (index == curr_index) {
        AST_Node* v = single_value_token(index);
        if (v) {
            ASTs.push_back(v);
        }
        if (v) {
            print_AST(v);
            cout << endl;
            evaluate_print(v);
            cout << endl;
        }
        index = curr_index + 2;
        return; 
    }

    size_t error_ = 0;
    bool a = check_error(index, curr_index, error_);
    if (!a) {
        a = check_assignment(index, curr_index, error_);
    }
    if (a) {
        cout << "Unexpected token at line 1 column " << tokens.at(error_)->column << ": " << tokens.at(error_)->raw_value << endl;
        index = curr_index + 2; 
        return;
    }
    root = read_one_line(index, curr_index, nullptr);
    if (!error) {
        ASTs.push_back(root);
    }
    else {
        delete_help(root);
    }
    if (root) {
        print_AST(root);
        cout << endl;
        evaluate_print(root);
        if (curr_index + 2 < tokens.size()) {
            cout << endl;
        }
    }
    index = curr_index + 2;
    error = false;
    // cout << "ended" << endl;
    // one line 
    // cout << "checking index: " << index << endl;
}

bool InfixParser::check_error(size_t begin_line, size_t end_line, size_t& error_index) {
    int count = 0;
    bool operator_last = false;
    bool last_left = 0;
    bool last_val = false;
    // one line included endtoken
    for (size_t i = begin_line; i <= end_line + 1; ++i) {
        if (operators.count(tokens.at(i)->raw_value)) {
            if (last_left) {
                error_index = i;
                return true;
            }
            if (operator_last) {
                error_index = i;
                return true;
            }
            else {
                operator_last = true;
            }
            if (i == begin_line) {
                error_index = i;
                return true;
            }
            else if (i == end_line) {
                error_index = i + 1;
                return true;
            }
            last_left = false;
            last_val = false;
        }
        else if (tokens.at(i)->raw_value == "(") {
            last_left = true;
            operator_last = false;
            ++count;
            if (last_val) {
                error_index = i;
                return true;
            }
            last_val = false;
        }
        else if (tokens.at(i)->raw_value == ")") {
            if (operator_last || last_left) {
                error_index = i;
                return true;
            }
            last_left = false;
            operator_last = false;
            --count;
            last_val = true;
        }
        else {
            last_left = false;
            operator_last = false;
            if (last_val && (tokens.at(i)->raw_value != "END")) {
                error_index = i;
                return true;
            }
            last_val = true;
        }
        if (count < 0) {
            error_index = i;
            return true;
        }
    }
    if (count != 0) {
        error_index = end_line + 1;
        return true;
    }
    return false;
}

bool InfixParser::check_assignment(size_t begin_line, size_t end_line, size_t& error_index) {
    if (begin_line == end_line && tokens.at(begin_line)->raw_value == "=") {
        error_index = begin_line;
        return true;
    }
    if (tokens.at(end_line)->raw_value == "=") {
        error_index = end_line + 1;
        return true;
    }
    for (size_t i = end_line; i > begin_line; --i) {
        if (tokens.at(i)->raw_value == "=") {
            if ((tokens.at(i + 1)->raw_value == ")") || (operators.count(tokens.at(i + 1)->raw_value))) {
                
                error_index = i + 1;
                return true;
            }
            else if (!isalpha(tokens.at(i - 1)->raw_value.at(0)) && (tokens.at(i - 1)->raw_value.at(0) != '_')){
                error_index = i;
                return true;
            }
        }
    }
    if (tokens.at(begin_line)->raw_value == "=") {
        error_index = begin_line;
        return true;
    }
    return false;
}

AST_Node* InfixParser::read_one_line(size_t begin_line, size_t end_line, AST_Node* in_parent) {
    if (index + 1 == tokens.size()) {
        return nullptr;
    }
    if (begin_line > end_line) {

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
        cout << endl;
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
    if (node->is_number) {
        cout << stod(node->data->raw_value);
    }
    else {
        cout << node->data->raw_value;
    }
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
    if (AST_Node::runtime_error) {
        AST_Node::runtime_error = false;
        Data::curr_variables.clear();
        return;
    }
    if (calculate.data_type == "DOUBLE") {
        cout << calculate.double_val;
        update_variables();
    }
    else if (calculate.data_type == "BOOL") {
        if (calculate.bool_val) {
            cout << "true";
        }
        else if (!calculate.bool_val) {
            cout << "false";
        }
        update_variables();
    }
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