
#include "AST.h"
#include "AST_Node.h"
#include <cmath>
#include <string>
using namespace std;

AST_Node::AST_Node() : left(nullptr), right(nullptr), parent(nullptr), data(nullptr), 
                        single_val(false), is_number(false), is_function(false), is_array(false), is_array_val(false)  {}

AST_Node::AST_Node(Token* in_data) : left(nullptr), right(nullptr), parent(nullptr), data(in_data), 
                        single_val(false), is_number(false),  is_function(false), is_array(false), is_array_val(false) {}

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

void AST_Node::update_array_elements() {
    if (!elements.empty()) {
        for (size_t i = 0; i < elements.size(); ++i) {
            val.array_elements.push_back(elements.at(i)->val);
        }
    }
}

Array_Fct::Array_Fct(Token* in_data) {
    data = in_data;
    is_function = false;
    is_array = false;
    is_array_val = false;
    is_number = false;
    single_val = true;
    val.actual_val = in_data->raw_value;
    val.data_type = "ARRAY_FCT";
    parameters = nullptr;
    index = nullptr;
}

Array::Array(Token* in_data) {
    data = in_data;
    is_function = false;
    is_array = true;
    is_array_val = false;
    is_number = false;
    val.actual_val = "ARRAY_NONE";
    val.data_type = "ARRAY";
    single_val = true;
    parameters = nullptr;
    index = nullptr;
}

Array_Val::Array_Val(Token* in_data) {
    data = in_data;
    is_function = false;
    is_array = false;
    is_array_val = true;
    is_number = false;
    val.actual_val = in_data->raw_value;
    val.data_type = "ARRAY_VAL";
    single_val = true;
    parameters = nullptr;
    index = nullptr;
}

Double_Operation::Double_Operation(Token* in_data){
    is_array = false;
    is_array_val = false;
    is_number = false;
    is_function = false;
    data = in_data;
    val.actual_val = in_data->raw_value;
    val.data_type = "DOUBLE";
    if (isdigit(in_data->raw_value.at(0))) {
        val.double_val = stod(in_data->raw_value);
    }
    single_val = false;
    parameters = nullptr;
    index = nullptr;
}

Boolean_Operation::Boolean_Operation(Token* in_data){
    is_array = false;
    is_array_val = false;
    is_number = false;
    is_function = false;
    val.actual_val = in_data->raw_value;
    val.data_type = "BOOL";
    data = in_data;
    single_val = false;
    parameters = nullptr;
    index = nullptr;
}

Direct_Val::Direct_Val(Token* in_data){
    val.actual_val = in_data->raw_value;
    is_array = false;
    is_array_val = false;
    is_number = false;
    is_function = false;
    if (in_data->raw_value == "true") {
        val.bool_val = true;
        val.data_type = "BOOL";
    }
    else if (in_data->raw_value == "false") {
        val.bool_val = false;
        val.data_type = "BOOL";
    }
    else if (in_data->raw_value == "null") {
        val.data_type = "NULL";
    }
    else {
        is_number = true;
        val.double_val = stod(in_data->raw_value);
        val.data_type = "DOUBLE";
    }
    data = in_data;
    single_val = true;
    parameters = nullptr;
    index = nullptr;
}

Variable_Val::Variable_Val(Token* in_data) {
    is_number = false;
    is_function = false;
    val.actual_val = in_data->raw_value;
    is_array = false;
    is_array_val = false;
    data = in_data;
    val.data_type = "EMPTY";
    single_val = true;
    if (AST_Node::prev_variables.find(in_data->raw_value) != AST_Node::prev_variables.end()) {
        val = prev_variables.at(in_data->raw_value);
    }
    parameters = nullptr;
    index = nullptr;
}

Assign::Assign(Token* in_data) {
    is_array = false;
    is_array_val = false;
    is_number = false;
    is_function = false;
    val.actual_val = in_data->raw_value; 
    single_val = false;
    data = in_data;
    parameters = nullptr;
    index = nullptr;
}

Comparison_Val::Comparison_Val(Token* in_data) {
    is_array = false;
    is_array_val = false;
    is_number = false;
    is_function = false;
    val.actual_val = in_data->raw_value;
    val.data_type = "BOOL";
    single_val = false;
    data = in_data;
    parameters = nullptr;
    index = nullptr;
}

Equality_Val::Equality_Val(Token* in_data) {
    is_array = false;
    is_array_val = false;
    is_number = false;
    is_function = false;
    single_val = false;
    val.actual_val = in_data->raw_value;
    val.data_type = "BOOL";
    data = in_data;
    parameters = nullptr;
    index = nullptr;
}

Data Array_Fct::get_value(Data& left_val, Data& right_val) {
    // need to change here
    if (runtime_error) {
        return Data();
    }
    left_val.data_type = "NONE";
    if ((data->raw_value == "len") || (data->raw_value == "pop")) {
        if (right_val.array_elements.size() != 1) {
            cout << "Runtime error: incorrect argument count.";
            runtime_error = true;
            return Data();
        }
    }
    else if (data->raw_value == "push") {
        if (right_val.array_elements.size() != 2) {
            cout << "Runtime error: incorrect argument count.";
            runtime_error = true;
            return Data();
        }
    }
    if (right_val.array_elements.at(0).data_type != "ARRAY") {
        cout << "Runtime error: not an array.";
        runtime_error = true;
        return Data();
    }
    Data a;
    if (data->raw_value == "len") {
        a.data_type = "DOUBLE";
        a.double_val = static_cast<double> (right_val.array_elements.at(0).array_elements.size());

    }
    else if (data->raw_value == "pop") {
        if (right_val.array_elements.at(0).array_elements.size() == 0) {
            cout << "Runtime error: underflow.";
            runtime_error = true;
            return Data();
        }
        a = right_val.array_elements.at(0).array_elements.at(right_val.array_elements.at(0).array_elements.size() - 1);
        right_val.array_elements.at(0).array_elements.pop_back();
        if (right_val.array_elements.at(0).actual_val != "ARRAY_NONE") {
            if (Data::curr_variables.find(right_val.array_elements.at(0).actual_val) != Data::curr_variables.end()) {
                Data::curr_variables.at(right_val.array_elements.at(0).actual_val) = right_val.array_elements.at(0);
            }
            else {
                Data::curr_variables.emplace(right_val.array_elements.at(0).actual_val, right_val.array_elements.at(0));
            }
        }
    }
    else if (data->raw_value == "push") {
        a.data_type = "NULL";
        right_val.array_elements.at(0).array_elements.push_back(right_val.array_elements.at(1));
        if (right_val.array_elements.at(0).actual_val != "ARRAY_NONE") {
            if (Data::curr_variables.find(right_val.array_elements.at(0).actual_val) != Data::curr_variables.end()) {
                Data::curr_variables.at(right_val.array_elements.at(0).actual_val) = right_val.array_elements.at(0);
            }
            else {
                Data::curr_variables.emplace(right_val.array_elements.at(0).actual_val, right_val.array_elements.at(0));
            }
        }
    }
    return a;
}

Data Array::get_value(Data& left_val, Data& right_val) {
    left_val.data_type = "NONE";
    right_val.data_type = "NONE";
    return val;
}

Data Array_Val::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    // cout << "beginning: " << left_val.double_val << endl;
    if (right_val.array_elements.empty() && (isalpha(data->raw_value.at(0)) || (data->raw_value.at(0) == '_'))) {
        if (Data::curr_variables.find(data->raw_value) != Data::curr_variables.end()) {
            right_val = Data::curr_variables.at(data->raw_value);
        }
        else if (prev_variables.find(data->raw_value) != prev_variables.end()) {
            right_val = prev_variables.at(data->raw_value);
        }
        // else {
        //     cout << "Runtime error: not an array.";
        //     runtime_error = true;
        //     return Data();
        // }
        if (right_val.data_type != "ARRAY") {
            cout << "Runtime error: not an array.";
            runtime_error = true;
            return Data();
        }
    }
    if (left_val.array_elements.size() != 1) {
        cout << "Runtime error: index is not a number.";
        // cout << 1 << endl;
        runtime_error = true;
        return Data();
    }
    else if (left_val.array_elements.at(0).data_type != "DOUBLE") {
        cout << "Runtime error: index is not a number.";
        runtime_error = true;
        // cout << 2 << endl;
        return Data();
    }
    double c = 1.0;
    double v = modf(left_val.array_elements.at(0).double_val, &c);
    if (v != 0.0) {
        cout << "Runtime error: index is not an integer.";
        runtime_error = true;
        return Data();
    }
    else if ((left_val.array_elements.at(0).double_val >= static_cast<double>(right_val.array_elements.size())) ||
                (left_val.array_elements.at(0).double_val < 0)) {
        cout << "Runtime error: index out of bounds.";
        runtime_error = true;
        return Data();
    }
    // cout << left_val.array_elements.at(0).double_val << endl;
    int index_array = static_cast<int>(left_val.array_elements.at(0).double_val);
    right_val.array_elements.at(index_array).index_val = left_val.array_elements.at(0).double_val;
    right_val.array_elements.at(index_array).actual_val = data->raw_value;
    right_val.array_elements.at(index_array).is_array_val_ = true;
    return right_val.array_elements.at(index_array);
}

Function_Val::Function_Val(vector<Token*> in_data) {
    is_number = false;
    is_function = true;
    val.actual_val = "";
    // cout << "before adding raw values" << endl; 
    for (size_t i = 0; i < in_data.size(); i++){
        // cout << "adding raw_values" << in_data.at(i)->raw_value << endl;
        val.actual_val += in_data.at(i)->raw_value;
        if (in_data.at(i)->raw_value == ","){
            val.actual_val += " ";
        } else {
            data_vec.push_back(in_data.at(i));
        }
    }
    // cout << "after creating function" << endl;   
    val.data_type = "EMPTY" ;
}

Data Double_Operation::get_value(Data& left_val, Data& right_val) {
    // cout << "double_operation: " << " left_val: " << left_val.data_type << " right_val: " << right_val.data_type << endl;
    if (runtime_error) {
        return Data();
    }
    if (invalid_variable(left_val) && !runtime_error) {
        cout << "here 3" << endl;
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
        cout << "here 4" << endl;
        cout << "Runtime error: unknown identifier " << right_val.actual_val;
        runtime_error = true;
        return Data();
    }
    if (((left_val.data_type != "DOUBLE") || (right_val.data_type != "DOUBLE")) && !runtime_error) {
        cout << "here 5" << endl;
        cout << "Runtime error: invalid operand type.";
        runtime_error = true;
        Data a;
        return a;
    }
    Data result("", "DOUBLE");
    result.actual_val = data->raw_value;
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
        cout << "here" << endl;
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
        cout << "here 2" << endl;
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
    result.actual_val = data->raw_value;
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
        result = Data::curr_variables.at(val.actual_val);
        result.actual_val = data->raw_value;
    }
    else if (prev_variables.find(val.actual_val) != prev_variables.end()) {
        result = prev_variables.at(val.actual_val);
        result.actual_val = data->raw_value;
    }
    // error 
    return result;
}

Data Equality_Val::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    if (invalid_variable(left_val) && !runtime_error) {
                // cout << "here 3" << endl;
        cout << "here 6" << endl;
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
                // cout << "here 4" << endl;
        cout << "here 7" << endl;
        cout << "Runtime error: unknown identifier " << right_val.actual_val;
        runtime_error = true;
        return Data();
    }
    Data result("", "BOOL");
    result.actual_val = data->raw_value;
    if (this->data->raw_value == "==") {
        result.bool_val = (left_val == right_val);
    }
    else if (this->data->raw_value == "!=") {
        result.bool_val = (!(left_val == right_val));
    }
    return result;
}

Data Assign::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    // cout << right_val.actual_val << endl;
    // cout << "type: " << right_val.data_type << endl;
    // cout << "enter assign" << endl;
    if ((!isalpha(left_val.actual_val.at(0)) && (left_val.actual_val.at(0) != '_')) &&
        (!left_val.is_array_val_)) {
        // cout << left_val.data_type << endl;
        cout << "Runtime error: invalid assignee.";
        runtime_error = true;
        Data a;
        return a;
    }
    if (invalid_variable(right_val) && !runtime_error) {
        // cout << "q" << endl;
                // cout << "here 5" << endl;
        cout << "here 8" << endl;
        cout << "Runtime error: unknown identifier " << right_val.actual_val;
        runtime_error = true;
        Data a;
        return a;
    }
    Data result;
    result.actual_val = left_val.actual_val;
    if (left_val.index_val == -1 || (!isalpha(left_val.actual_val.at(0)) && (left_val.actual_val.at(0) != '_'))) {
        result.data_type = right_val.data_type;
        result.double_val = right_val.double_val;
        result.bool_val = right_val.bool_val;
        result.array_elements = right_val.array_elements;
        if (left_val.index_val != -1) {
            return result;
        }
    }
    else {
        // cout << "result.actual_val" << result.actual_val << endl;
        if (Data::curr_variables.find(result.actual_val) != Data::curr_variables.end()) {
            result = Data::curr_variables.at(result.actual_val);
            // cout << 100 << endl;
        }
        else if (prev_variables.find(result.actual_val) != prev_variables.end()) {
            result = prev_variables.at(result.actual_val);
            // cout << 101 << endl;
        }
        // cout << "here" << endl;
        // cout << left_val.index_val << endl;
        // cout << result.array_elements.size() << endl;
        result.array_elements.at(left_val.index_val) = right_val; 
    }
    if (Data::curr_variables.find(left_val.actual_val) != Data::curr_variables.end()) {
        Data::curr_variables.at(left_val.actual_val) = result;
    }
    else {
        Data::curr_variables.emplace(left_val.actual_val, result);
    }
    result.actual_val = data->raw_value;
    // cout << "check for map curr_variables " << Data::curr_variables.at(left_val.actual_val).double_val << endl;
    if (left_val.index_val != -1) {
        return result.array_elements.at(left_val.index_val);
    }
    return result;
}

Data Comparison_Val::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    if (invalid_variable(left_val) && !runtime_error) {
        cout << "9" << endl;
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
        cout << "10" << endl;
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
    result.actual_val = data->raw_value;
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

Data Function_Val::get_value(Data& left_val, Data& right_val) {
    if (runtime_error) {
        return Data();
    }
    if (invalid_variable(left_val) && !runtime_error) {
        cout << "here 11" << endl;
        cout << "Runtime error: unknown identifier " << left_val.actual_val;
        runtime_error = true;
        return Data();
    }
    else if (invalid_variable(right_val) && !runtime_error) {
        cout << "here 12" << endl;
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
    result.actual_val = data->raw_value;
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