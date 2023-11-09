#include "AST.h"
#include "AST_Node.h"
#include <cmath>
#include <string>
using namespace std;

AST_Node::AST_Node() : left(nullptr), right(nullptr), parent(nullptr), data(nullptr), 
                        single_val(false), is_number(false) {}

AST_Node::AST_Node(Token* in_data) : left(nullptr), right(nullptr), parent(nullptr), data(in_data), 
                        single_val(false), is_number(false) {}

AST_Node::~AST_Node() { }

// Data AST_Node::get_value(Data& left_val, Data& right_val) {
//     left_val.actual_val = "base";
//     right_val.actual_val = "base";
//     return Data();
// }

bool AST_Node::is_variable(Data& a) const {
    if (Data::curr_variables.find(a.actual_val) != Data::curr_variables.end()) {
        return true;
    }
    else if (AST_Node::prev_variables.find(a.actual_val) != AST_Node::prev_variables.end()) {
        return true;
    }
    return false;
}

bool AST_Node::invalid_variable(Data& a) const {
    if (a.data_type == "EMPTY") {
        return true;
    }
    return false;
}


map<string, Data> AST_Node::prev_variables;

bool AST_Node::runtime_error = false;

Double_Operation::Double_Operation(Token* in_data){
    is_number = false;
    data = in_data;
    val.actual_val = in_data->raw_value;
    val.data_type = "DOUBLE";
    if (isdigit(in_data->raw_value.at(0))) {
        val.double_val = stod(in_data->raw_value);
    }
}

Boolean_Operation::Boolean_Operation(Token* in_data){
    is_number = false;
    val.actual_val = in_data->raw_value;
    val.data_type = "BOOL";
    data = in_data;
}

Direct_Val::Direct_Val(Token* in_data){
    val.actual_val = in_data->raw_value;
    is_number = false;
    if (in_data->raw_value == "true") {
        val.bool_val = true;
        val.data_type = "BOOL";
    }
    else if (in_data->raw_value == "false") {
        val.bool_val = false;
        val.data_type = "BOOL";
    }
    else {
        is_number = true;
        val.double_val = stod(in_data->raw_value);
        val.data_type = "DOUBLE";
    }
    data = in_data;
}

Variable_Val::Variable_Val(Token* in_data) {
    is_number = false;
    val.actual_val = in_data->raw_value;
    data = in_data;
    val.data_type = "EMPTY";
    if (AST_Node::prev_variables.find(in_data->raw_value) != AST_Node::prev_variables.end()) {
        val = prev_variables.at(in_data->raw_value);
    }
}

Assign::Assign(Token* in_data) {
    is_number = false;
    val.actual_val = in_data->raw_value; 
    data = in_data;
}

Comparison_Val::Comparison_Val(Token* in_data) {
    is_number = false;
    val.actual_val = in_data->raw_value;
    val.data_type = "BOOL";
    data = in_data;
}

Equality_Val::Equality_Val(Token* in_data) {
    is_number = false;
    val.actual_val = in_data->raw_value;
    val.data_type = "BOOL";
    data = in_data;
}


Data Double_Operation::get_value(Data& left_val, Data& right_val) {
    // cout << "double_operation: " << " left_val: " << left_val.data_type << " right_val: " << right_val.data_type << endl;
    if (runtime_error) {
        return Data();
    }
    if (invalid_variable(left_val) && !runtime_error) {
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
        cout << "Runtime error: unknown identifier " << right_val.actual_val;
        runtime_error = true;
        return Data();
    }
    if (((left_val.data_type != "DOUBLE") || (right_val.data_type != "DOUBLE")) && !runtime_error) {
        cout << "Runtime error: invalid operand type.";
        runtime_error = true;
        Data a;
        return a;
    }
    Data result("", "DOUBLE");
    if (this->data->raw_value == "+") {
        result.double_val = left_val.double_val + right_val.double_val;
    }
    else if (this->data->raw_value == "-") {
        result.double_val = left_val.double_val - right_val.double_val;
    }    
    else if (this->data->raw_value == "*") {
        result.double_val = left_val.double_val * right_val.double_val;
    }    
    else if (this->data->raw_value == "/") {
        if (right_val.double_val == 0) {
            cout << "Runtime error: division by zero.";
            runtime_error = true;
            Data err;
            return err;
        }
        result.double_val = left_val.double_val / right_val.double_val;   
    }
    else if (this->data->raw_value == "%") {
        if (right_val.double_val == 0) {
            Data err;
            return err;
        }
        result.double_val = fmod(left_val.double_val, right_val.double_val);
    }
    return result;
}

Data Boolean_Operation::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    if (invalid_variable(left_val) && !runtime_error) {
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
        cout << "Runtime error: unknown identifier " << right_val.actual_val;
        runtime_error = true;
        return Data();
    }
    if (((left_val.data_type != "BOOL") || (right_val.data_type != "BOOL")) && !runtime_error) {
        cout << "Runtime error: invalid operand type.";
        runtime_error = true;
        Data a;
        return a;
    }
    Data result("", "BOOL");
    result.data_type = "DOUBLE";
    if (this->data->raw_value == "|") {
        result.bool_val = (left_val.bool_val || right_val.bool_val);
    }
    else if (this->data->raw_value == "^") {
        if ((left_val.bool_val && !right_val.bool_val) || (!left_val.bool_val && right_val.bool_val)) {
            result.bool_val = true;
        }
        else {
            result.bool_val = false;
        }
    }    
    else if (this->data->raw_value == "&") {
        result.bool_val = (left_val.bool_val && right_val.bool_val);
    }  
    return result;
}

Data Direct_Val::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    left_val.data_type = "NONE";
    right_val.data_type = "NONE";
    return val;
}

Data Variable_Val::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    left_val.data_type = "NONE";
    right_val.data_type = "NONE";
    Data result;
    result.actual_val = data->raw_value;
    result.data_type = "EMPTY";
    if (Data::curr_variables.find(val.actual_val) != Data::curr_variables.end()) {
        return Data::curr_variables.at(val.actual_val);
    }
    else if (prev_variables.find(val.actual_val) != prev_variables.end()) {
        return prev_variables.at(val.actual_val);
    }
    // error 
    return result;
}

Data Equality_Val::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    if (invalid_variable(left_val) && !runtime_error) {
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
        cout << "Runtime error: unknown identifier " << right_val.actual_val;
        runtime_error = true;
        return Data();
    }
    if (((left_val.data_type != right_val.data_type) || (left_val.data_type == "NONE")) && !runtime_error) {
        cout << "runtime error: invalid operand type.";
        runtime_error = true;
        Data a;
        return a;
    }
    Data result("", "BOOL");
    if (this->data->raw_value == "==") {
        if (left_val.data_type == "BOOL") {
            result.bool_val = (left_val.bool_val == right_val.bool_val);
        }
        else if (left_val.data_type == "DOUBLE") {
            result.bool_val = (left_val.double_val == right_val.double_val);
        }
    }
    else if (this->data->raw_value == "!=") {
        if (left_val.data_type == "BOOL") {
            result.bool_val = (left_val.bool_val != right_val.bool_val);
        }
        else if (left_val.data_type == "DOUBLE") {
            result.bool_val = (left_val.double_val != right_val.double_val);
        }
    }
    return result;
}

Data Assign::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    // cout << right_val.actual_val << endl;
    // cout << "type: " << right_val.data_type << endl;
    if (invalid_variable(right_val) && !runtime_error) {
        // cout << "q" << endl;
        cout << "Runtime error: unknown identifier " << right_val.actual_val;
        runtime_error = true;
        Data a;
        return a;
    }
    Data result;
    result.actual_val = left_val.actual_val;
    result.data_type = right_val.data_type;
    // cout << "result.actual" << result.actual_val << endl;
    // cout << "result.datatype: " << result.data_type << endl;
    result.double_val = right_val.double_val;
    result.bool_val = right_val.bool_val;
    if (Data::curr_variables.find(left_val.actual_val) != Data::curr_variables.end()) {
        Data::curr_variables.at(left_val.actual_val) = result;
    }
    else {
        Data::curr_variables.emplace(left_val.actual_val, result);
    }
    // cout << "check for map curr_variables " << Data::curr_variables.at(left_val.actual_val).double_val << endl;
    return result;
}

Data Comparison_Val::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    if (invalid_variable(left_val) && !runtime_error) {
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
        cout << "Runtime error: unknown identifier " << right_val.actual_val;
        runtime_error = true;
        return Data();
    }
    if (((left_val.data_type != "DOUBLE") || (right_val.data_type != "DOUBLE")) && !runtime_error) {
        cout << "Runtime error: invalid operand type.";
        runtime_error = true;
        Data a;
        return a;
    }

    Data result("", "BOOL");
    if (this->data->raw_value == "<") {
        result.bool_val = left_val.double_val < right_val.double_val;
    }
    else if (this->data->raw_value == "<=") {
        result.bool_val = left_val.double_val <= right_val.double_val;
    }    
    else if (this->data->raw_value == ">") {
        result.bool_val = left_val.double_val > right_val.double_val;
    }  
    else if (this->data->raw_value == ">=") {
        result.bool_val = left_val.double_val >= right_val.double_val;
    
    }  
    return result;
}
