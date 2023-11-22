#include "InfixParser.h"
#include "AST_Node.h"
#include "Lexer.h"
#include <cctype>
#include <string>
#include <vector>
using namespace std;

InfixParser::InfixParser(vector<Token*>& tokens) {
    index = 0;
    this->tokens = tokens;
    operators = {"+", "-", "*", "/", "%", "&", "|", "^", "<", "<=", ">", ">=", "!=", "==", "="};
    array_functions = {"len", "pop", "push"};
    error = false;
    result_double = 0;
    result_bool = false;
}
InfixParser::InfixParser(){
    index = 0;
    operators = {"+", "-", "*", "/", "%", "&", "|", "^", "<", "<=", ">", ">=", "!=", "==", "="};
    array_functions = {"len", "pop", "push"};
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
    // cout << "read_all_token() enter" << endl;

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
    // cout << "enter sing_value_token" << endl;
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
        // cout << "enter here variable" << endl;
        Variable_Val* new_val = new Variable_Val(tokens.at(begin_a));
        new_val->single_val = true;
        return new_val;
    }
    else {
        // cout << "enter here number" << endl;
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
    // cout << "read_token() enter" << endl;
    if (index >= tokens.size()) {
        // cout << "return read_token" << endl;
        return;
    }
    size_t curr_index = index;
    // cout << "currIndex1: " << curr_index << endl;
    // cout << "1 Index here: " << index<< "  currIndex: " << curr_index << endl;
    AST_Node* root = nullptr;
    while (tokens.at(curr_index)->raw_value != "END") {
        ++curr_index;
    }
    // cout << "currIndex2: " << curr_index << endl;
    // cout << "2 Index here: " << index<< "  currIndex: " << curr_index << endl;
    curr_index -= 1;
    if (calc) {
        if (check_for_statement(index, curr_index)) {
            index = curr_index + 2;
            return;
        }
    }
    // cout << "currIndex3: " << curr_index << endl;
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
    // cout << "index: " << index << endl;
    // cout << "curr_index: " << curr_index << endl;
    // cout << "1 Index here: " << index<< "  currIndex: " << curr_index << endl;
    // cout << "1 Index here: " << index<< "  currIndex: " << curr_index << endl;
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
    // cout << "Here++++++++++++++++++++++++" << endl;

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
        cout << endl;
        // if (curr_index + 2 < tokens.size()) {
        //     cout << endl;
        // }
    }
    index = curr_index + 2;
    error = false;
    // cout << "ended" << endl;
    // one line 
    // cout << "checking index: " << index << endl;
}

bool InfixParser::check_array(size_t begin_line, size_t end_line) {
    int count = 0;
    for (size_t i = begin_line; i <= end_line; ++i) {
        if (tokens.at(i)->raw_value == "[") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count;
        }
        if ((count == -1) &&  (i != end_line)) {
            return false;
        }
    }
    return true;
}


// check if the value is array val or array
bool InfixParser::check_array_val(size_t begin_line, size_t end_line) {
    bool a = false;
    int count = 0;
    bool b = false;
    for (size_t i = begin_line; i <= end_line; ++i) {
        if (tokens.at(i)->raw_value == "[") {
            ++count;
            if (count == 0 && !a) {
                a = true;
                b = true;
            }
            else if (count == 0) {
                b = false;
            }
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count;
        }
        else {
            if (count < 0) {
                return false;
            }
        }
    }
    return b;
}

bool InfixParser::check_error(size_t begin_line, size_t end_line, size_t& error_index) {
    int count = 0;
    int count_b = 0;
    bool operator_last = false;
    bool last_left = false;
    bool last_val = false;
    bool is_function = false;
    bool last_left_b = false;
    bool is_fct = false;
    int fct_p = 0;
    // one line included endtoken
    for (size_t i = begin_line; i <= end_line + 1; ++i) {
        if(tokens.at(i)->is_function){
            // cout << "here token " << tokens.at(i)->raw_value << endl;
            is_function = true;
        }
        // cout << is_function << endl;
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
            last_left_b = false;
            last_val = false;
        } 
        else if (tokens.at(i)->raw_value == "(" && is_function){
            if(tokens.at(i + 1)->raw_value == ","){
                error_index = i;
                return true;
            }
            ++count;
            is_function = true;
            last_left = false;
            operator_last = false;
            last_val = true;
        }
        else if (array_functions.count(tokens.at(i)->raw_value)) {
            if (i + 2 > end_line) {
                error_index = i;
                return true;
            }
            if (tokens.at(i + 1)->raw_value != "(") {
                error_index = i + 1;
                return true;
            }
            last_left = false;
            is_fct = true;
            fct_p = count;
            last_val = false;
            last_left_b = false;
            operator_last = true;
        }
        else if (tokens.at(i)->raw_value == "(") {
            // cout << "here left paratheses" << endl;
            last_left = true;
            operator_last = false;
            ++count;
            if (last_val) {
                error_index = i;
                return true;
            }
            last_val = false;
            last_left_b = false;
        }
        else if (tokens.at(i)->raw_value == ")") {
            if ((fct_p + 1 == count) && is_fct) {
                is_fct = false;
            }
            else if (operator_last || last_left || last_left_b) {
                error_index = i;
                // cout << "here exit" << endl;
                return true;
            }
            last_left = false;
            operator_last = false;
            --count;
            last_val = true;
            if (count == 0){
                is_function = false;
            }
        } else if (is_function){
            last_left = false;
            operator_last = false;
            last_val = false;
            last_left_b = false;
        }
        else if (tokens.at(i)->raw_value == ",") {
            if (((count_b < 1) && !is_fct) || operator_last || last_left || last_left_b) {
                // cout << "error here" << endl;
                error_index = i;
                return true;
            }
            last_left = false;
            operator_last = false;
            last_val = false;
            last_left_b = false;
        }
        else if (tokens.at(i)->raw_value == "[") {
            last_left_b = false;
            last_left = false;
            operator_last = false;
            last_val = false;
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            if (operator_last || last_left) {
                error_index = i;
                return true;
            }
            last_left = false;
            operator_last = false;
            last_left_b = false;
            last_val = true;
            --count_b;
        }
        else {
            // cout << "exit here" << tokens.at(index)->raw_value << endl;
            last_left = false;
            operator_last = false;
            if (last_val && (tokens.at(i)->raw_value != "END")) {
                error_index = i;
                return true;
            }
            last_val = true;
        }
        // cout << tokens.at(i)->raw_value << endl;
        if (count < 0) {
            error_index = i;
            return true;
        }
        else if (count_b < 0) {
            error_index = i;
            return true;
        }
    }
    if ((count != 0) || (count_b != 0)) {
        error_index = end_line + 1;
        return true;
    }
    return false;
}

bool InfixParser::check_assignment(size_t begin_line, size_t end_line, size_t& error_index) {
    // cout << "begin_line " << begin_line << " end_line: " << end_line << endl;
    if (begin_line == end_line && tokens.at(begin_line)->raw_value == "=") {
        error_index = begin_line;
        return true;
    }
    if (tokens.at(end_line)->raw_value == "=") {
        error_index = end_line + 1;
        // cout << "p" << endl;
        return true;
    }
    // cout << "here" << endl;
    for (size_t i = end_line; i > begin_line; --i) {
        // cout << "a" << endl;
        if (tokens.at(i)->raw_value == "=") {
            if ((tokens.at(i + 1)->raw_value == ")") || (operators.count(tokens.at(i + 1)->raw_value)) || (tokens.at(i + 1)->raw_value == "]")) {
                
                error_index = i + 1;
                // cout << "here 1 " << endl;
                return true;
            }
            else if (tokens.at(i - 1)->raw_value == "(" || operators.count(tokens.at(i - 1)->raw_value) || (tokens.at(i - 1)->raw_value == "[")){
                error_index = i;
                // cout << "return here" << endl;
                return true;
            }
        }
        // cout << "b" << endl;
    }
    if (tokens.at(begin_line)->raw_value == "=") {
        error_index = begin_line;
        // cout << "00" << endl;
        return true;
    }
    return false;
}

bool InfixParser::check_single_array_function(size_t begin_line, size_t end_line) {
    int count = 0;
    bool is_first = false;
    for (size_t i = begin_line; i <= end_line; ++i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && !is_first && (i == end_line)) {
            return true;
        }
        if (count == 0) {
            is_first = true;
        }
    }
    return false;
}

AST_Node* InfixParser::read_one_line(size_t begin_line, size_t end_line, AST_Node* in_parent) {
    // cout << tokens.at(begin_line)->raw_value << endl;
    // cout << "enter" << endl;
    if (index + 1 == tokens.size()) {
        return nullptr;
    }
    if (begin_line > end_line) {
        // cout << "here" << endl;
        // error here for parenthesis () 
        return nullptr;
    }
    if (begin_line == end_line) {
        // direct values such as numbers, true/false, variables
        if (isalpha(tokens.at(begin_line)->raw_value.at(0)) && (tokens.at(begin_line)->raw_value != "true") && (tokens.at(begin_line)->raw_value != "false")
            && (tokens.at(begin_line)->raw_value != "null")) {
            //variable
            Variable_Val* add_variable = new Variable_Val(tokens.at(begin_line));
            add_variable->single_val = true;
            return add_variable;
        }
        else if (isdigit((tokens.at(begin_line)->raw_value).at(0)) || (tokens.at(begin_line)->raw_value == "true") || (tokens.at(begin_line)->raw_value == "false")
                || (tokens.at(begin_line)->raw_value == "null")) {
            Direct_Val* direct_val = new Direct_Val(tokens.at(begin_line));
            direct_val->single_val = true;
            return direct_val;
        }
        // error not numbers, true/false, or variables
        return nullptr;
    }

    if ((tokens.at(begin_line)->raw_value == "[")  && (tokens.at(end_line)->raw_value == "]")) {
        bool x = check_array_val(begin_line + 1, end_line);
        bool y = check_array(begin_line + 1, end_line);
        // if (x) {
        //     cout << "true" << endl;
        // }
        if (x) {
            // array val
            Array_Val* add_array_val = new Array_Val(tokens.at(begin_line));
            size_t start_element_index = begin_line + 1;
            size_t start_access_index = begin_line + 1;
            AST_Node* add_element = nullptr;
            int count_array = 0;
            if (tokens.at(begin_line + 1)->raw_value == "]") {
                start_access_index = begin_line + 2;
            }
            else {
                // cout << "here" << endl;
                for (size_t w = begin_line + 1; w <= end_line; ++w) {
                    if (tokens.at(w)->raw_value == "[") {
                        ++count_array;
                    }
                    else if (tokens.at(w)->raw_value == "]") {
                        --count_array;
                    }
                    if (((tokens.at(w)->raw_value == ",") && (count_array == 0)) || (count_array == -1)) {
                        // // cout << "here checking: ";
                        // for (size_t v = start_element_index; v < w; ++v) {
                        //     cout << tokens.at(v)->raw_value << " ";
                        // }
                        // cout << "done checking" << endl;
                        add_element = read_one_line(start_element_index, w - 1, add_array_val);
                        add_array_val->elements.push_back(add_element);
                        start_element_index = w + 1;
                        // cout << start_element_index << endl;
                        // cout << "children num: " << add_array_val->elements.size() << endl;
                    }
                    if (count_array == -1) {
                        start_access_index = w + 1;
                        break;
                    }
                }
                add_array_val->update_array_elements();
            }
            add_array_val->index = read_one_line(start_access_index, end_line, add_array_val);
            add_array_val->single_val = true;
            return add_array_val;
        }
        else if (y) {
            // array
            // cout << "array" << endl;
            Array* add_array = new Array(tokens.at(begin_line));
            if (begin_line + 1 == end_line) {
                return add_array;
            }
            size_t start_element_index = begin_line + 1;
            AST_Node* add_element = nullptr;
            for (size_t w = begin_line + 1; w <= end_line; ++w) {
                if ((tokens.at(w)->raw_value == ",") || (w == end_line)) {
                    // cout << "checking: ";
                    // cout << tokens.at(w - 1)->raw_value << endl;
                    add_element = read_one_line(start_element_index, w - 1, add_array);
                    add_array->elements.push_back(add_element);
                    // cout << add_element->data->raw_value << endl;
                    start_element_index = w + 1;
                }
            }
            // cout << "finished" << endl;
            add_array->update_array_elements();
            return add_array;
        }
    }

    if (isalpha(tokens.at(begin_line)->raw_value.at(0)) && (tokens.at(begin_line + 1)->raw_value == "[") && (tokens.at(end_line)->raw_value == "]")) {
        bool is_val = check_array(begin_line + 2, end_line);
        if (is_val) {
            // cout << "is here" << endl;
            Array_Val* add_array_val = new Array_Val(tokens.at(begin_line));
            add_array_val->index = read_one_line(begin_line + 1, end_line, add_array_val);
            return add_array_val;
        }
    }

    if (array_functions.count(tokens.at(begin_line)->raw_value)) {
            bool m = check_single_array_function(begin_line + 1, end_line);
            if (m) {
                Array_Fct* add_array_fct = new Array_Fct(tokens.at(begin_line));
                add_array_fct->parameters = new Array(tokens.at(begin_line + 1));
                if (begin_line + 2 == end_line) {
                    return add_array_fct;
                }
                size_t start_p = begin_line + 2;
                int co = 0;
                int co_b = 0;
                // cout << "parameters" << endl;
                // cout << "end_line: " << end_line << endl;
                for (size_t w = begin_line + 2; w <= end_line; ++w) {
                    // cout << tokens.at(w)->raw_value << endl;
                    // cout << "index: " << w << endl;
                    if (tokens.at(w)->raw_value == "(") {
                        ++co;
                    }
                    else if (tokens.at(w)->raw_value == "[") {
                        ++co_b;
                    }
                    else if (tokens.at(w)->raw_value == "]") {
                        --co_b;
                    }
                    else if (tokens.at(w)->raw_value == ")") {
                        --co;
                    }
                    if (((co == 0) && (co_b == 0) && (tokens.at(w)->raw_value == ",")) || (w == end_line)) {
                        // cout << "here" << endl;
                        add_array_fct->parameters->elements.push_back(read_one_line(start_p, w - 1, add_array_fct->parameters));
                        start_p = w + 1;
                    }
                    // cout << "last" << endl;
                }
                // cout << "k" << endl;
                return add_array_fct;
            }
    }


    int count = 0;
    int count_b = 0;
    for (size_t i = begin_line; i <= end_line; ++i) {
        // cout << "first: " << i << endl;
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == "[") {
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count_b;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "=")) {
            // cout << "equal" << endl;
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
    count_b = 0;
    // cout << "end_line: " << end_line << endl;
    // cout << "begin_line: " << begin_line << endl;
    if(isalpha(tokens.at(begin_line)->raw_value.at(0)) && (tokens.at(begin_line)->raw_value != "true") && (tokens.at(begin_line)->raw_value != "false") && tokens.at(begin_line)->is_function){
        vector <Token*> function_call;
        function_call.push_back(tokens.at(begin_line));
        size_t count = 0;
        size_t i = begin_line;
        do {
            i++;
            if(tokens.at(i)->raw_value == "("){
                count++;
            } else if (tokens.at(i)->raw_value == ")"){
                count--;
            }
            // cout << tokens.at(i)->raw_value << endl;
            function_call.push_back(tokens.at(i));
        } while(tokens.at(i)->raw_value != ")" || count != 0);
        // cout << "exit statement" << endl;
        Function_Val* add_function = new Function_Val(function_call);
        // cout << "here" << endl;
        add_function->parent = in_parent;
        add_function->single_val = true;
        // cout << "here 2" << endl;
        return add_function;
    }// direct values such as numbers, true/false, variables
    Boolean_Operation* add_bool = nullptr;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        else if (tokens.at(i)->raw_value == "[") {
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count_b;
        }
        if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "|")) {
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
    count_b = 0;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        else if (tokens.at(i)->raw_value == "[") {
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count_b;
        }
        if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "^")) {
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
    count_b = 0;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        else if (tokens.at(i)->raw_value == "[") {
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count_b;
        }
        if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "&")) {
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
    count_b = 0;
    Equality_Val* new_equal = nullptr;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        else if (tokens.at(i)->raw_value == "[") {
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count_b;
        }
        if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "!=")) {
            new_equal = new Equality_Val(tokens.at(i));
            new_equal->parent = in_parent;
            new_equal->left = read_one_line(begin_line, i - 1, new_equal);
            new_equal->right = read_one_line(i + 1, end_line, new_equal);
            if (!new_equal->left || !new_equal->right) {
                delete_help(new_equal);
            }
            return new_equal;
        }
        else if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "==")) {
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
    count_b = 0;
    Comparison_Val* new_compare = nullptr;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        else if (tokens.at(i)->raw_value == "[") {
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count_b;
        }
        if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == ">=")) {
            new_compare = new Comparison_Val(tokens.at(i));
            new_compare->parent = in_parent;
            new_compare->left = read_one_line(begin_line, i - 1, new_compare);
            new_compare->right = read_one_line(i + 1, end_line, new_compare);
            if (!new_compare->left || !new_compare->right) {
                delete_help(new_compare);
            }
            return new_compare;
        }
        else if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == ">")) {
            new_compare = new Comparison_Val(tokens.at(i));
            new_compare->parent = in_parent;
            new_compare->left = read_one_line(begin_line, i - 1, new_compare);
            new_compare->right = read_one_line(i + 1, end_line, new_compare);
            if (!new_compare->left || !new_compare->right) {
                delete_help(new_compare);
            }
            return new_compare;
        }
        else if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "<=")) {
            new_compare = new Comparison_Val(tokens.at(i));
            new_compare->parent = in_parent;
            new_compare->left = read_one_line(begin_line, i - 1, new_compare);
            new_compare->right = read_one_line(i + 1, end_line, new_compare);
            if (!new_compare->left || !new_compare->right) {
                delete_help(new_compare);
            }
            return new_compare;
        } 
        else if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "<")) {
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
    count_b = 0;
    Double_Operation* double_add = nullptr;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        else if (tokens.at(i)->raw_value == "[") {
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count_b;
        }
        if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "-")) {
            double_add = new Double_Operation(tokens.at(i));
            double_add->parent = in_parent;
            double_add->left = read_one_line(begin_line, i - 1, double_add);
            double_add->right = read_one_line(i + 1, end_line, double_add);
            if (!double_add->left || !double_add->right) {
                delete_help(double_add);
            }
            return double_add;
        }
        else if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "+")) {
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
    count_b = 0;
    for (size_t i = end_line; i >= begin_line; --i) {
        if (tokens.at(i)->raw_value == "(") {
            ++count;
        }
        else if (tokens.at(i)->raw_value == ")") {
            --count;
        }
        else if (tokens.at(i)->raw_value == "[") {
            ++count_b;
        }
        else if (tokens.at(i)->raw_value == "]") {
            --count_b;
        }
        if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "%")) {
            double_add = new Double_Operation(tokens.at(i));
            double_add->parent = in_parent;
            double_add->left = read_one_line(begin_line, i - 1, double_add);
            double_add->right = read_one_line(i + 1, end_line, double_add);
            if (!double_add->left || !double_add->right) {
                delete_help(double_add);
            }
            return double_add;
        }
        else if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "/")) {
            double_add = new Double_Operation(tokens.at(i));
            double_add->parent = in_parent;
            double_add->left = read_one_line(begin_line, i - 1, double_add);
            double_add->right = read_one_line(i + 1, end_line, double_add);
            if (!double_add->left || !double_add->right) {
                delete_help(double_add);
            }
            return double_add;
        }
        else if ((count == 0) && (count_b == 0) && (tokens.at(i)->raw_value == "*")) {
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
    // cout << "delete left" << endl;
    // cout << in_node->val.actual_val << endl;
    if (in_node->left){
        delete_help(in_node->left);
    }
    if (in_node->right){
        delete_help(in_node->right);
    }
    // cout << "delete right" << endl;
    if (!in_node->elements.empty()) {
        for (size_t i = 0; i < in_node->elements.size(); ++i) {
            delete_help(in_node->elements.at(i));
        }
    }
    if (in_node->index) {
        delete_help(in_node->index);
    }
    if (in_node->parameters) {
        delete_help(in_node->parameters);
    }
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
    // cout << "here" << endl;
    if (!node) {
        return;
    }
    // cout << "is function" << endl;
    // cout << "node->is_function" << endl;
    if (node->is_function){
        cout << node->val.actual_val;
    }
    // cout << "is_array" << endl;
    if (node->is_array) {
        cout << "[";
    }
    else if (!node->single_val) {
        cout << "(";
    }
    // cout << "is_parameters" << endl;
    if (node->parameters && !node->is_function) {
        // cout << "here" << endl;
        cout << node->data->raw_value;
        cout << "(";
        if (!node->parameters->elements.empty()) {
            // cout << "not empty" << endl;
            for (size_t j = 0; j < node->parameters->elements.size(); ++j) {
                print_AST(node->parameters->elements.at(j));
                if (j + 1 != node->parameters->elements.size()) {
                    cout << ", ";
                }
            }
        }
        // else {
        //     cout << "empty" << endl;
        // }
        cout << ")";
    } else if (node->is_function){
        // cout << "is_function" << endl;
    }
    else if (!node->is_array && !node->is_array_val) {
        // cout << "not array" << endl;
        print_AST(node->left);
        if (!node->single_val) {
            cout << " ";
        }
        if  (node->is_number) {
            cout << stod(node->data->raw_value);
        }
        else {
            cout << node->data->raw_value;
        }
        if (!node->single_val) {
            cout << " ";
        }
        print_AST(node->right);
    }
    else if (node->is_array) {
        // cout << "is array" << endl;
        if (!node->elements.empty()) {
            for (size_t i = 0; i < node->elements.size(); ++i) {
                print_AST(node->elements.at(i));
                if (i + 1 != node->elements.size()) {
                    cout << ", ";
                }
            }
        }
    }
    else {
        // cout << "elements" << endl;
        if (!node->elements.empty()) {
            cout << "[";
            for (size_t i = 0; i < node->elements.size(); ++i) {
                print_AST(node->elements.at(i));
                if (i + 1 != node->elements.size()) { 
                    cout << ", ";
                }
            }
            cout << "]";
        }
        else {
            cout << node->data->raw_value;
        }
        if (node->index) {
            print_AST(node->index);
        }
    }
    if (node->is_array) {
        cout << "]";
    }
    else if (!node->single_val) {
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
    if (in_node->parameters) {
        right_val = evaluate(in_node->parameters);
    }
    else if (in_node->is_array || in_node->is_array_val) {
        if (AST_Node::runtime_error) {
            return Data();
        }
        Data result;
        if (in_node->is_array) {
            result.data_type = "ARRAY";
        }
        else {
            result.data_type = "ARRAY_VAL";
        }
        if (!in_node->elements.empty()) {
            for (size_t i = 0; i < in_node->elements.size(); ++i) {
                result.array_elements.push_back(evaluate(in_node->elements.at(i)));
            }
            if (in_node->is_array) {
                return result;
            }
        }
        if (in_node->is_array_val) {
            left_val = evaluate(in_node->index);
            // cout << left_val.double_val << endl;
        }
        right_val = result;
    }
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
    cout << calculate;
    update_variables();
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

bool InfixParser::isFunction(string functionName){
    for(size_t i = 0; i < functionNames.size(); i++){
        if(functionName == functionNames.at(i)->raw_value){
            return true;
        }
    }
    return false;
}