#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <iomanip>
#include "Lexer.h"
#include <algorithm>
#include <cctype>

using namespace std; 

Lexer::Lexer(){
    // change here
    this->possible_values = {'=', '(', ')', '+', '-', '*', '/', '%', '{', '}', '|', '&', '^', ',', ';', ']', '['};
    this->error = false;
    inequalities = {'<', '>', '!'};
    string line = "";
    num_line = 0;
    while (!cin.eof()){
        ++num_line;
        getline(cin, line);
        this->whole_input.push_back(line);
        // for testing
        // cout << line << endl;
    }
}

Lexer::~Lexer(){
}

void Lexer::create_tokens(){
    string raw_value = "";
    int row = 1; 
    int column = 1;
    int prev_index = 1;
    bool variable = false;
    bool last_digit = false;
    // Token* add_token = nullptr;
    bool last_inequalities = false;
    bool double_equal = false;
    

    for(size_t i = 0; i < whole_input.size(); i++){
        for(size_t j = 0; j < whole_input.at(i).length(); j++){
            if (inequalities.count(whole_input.at(i).at(j)) || last_inequalities || double_equal) { // inequality
                if (last_inequalities || double_equal) { // before last_inequality
                    Token* new_token = new Token();
                    raw_value += whole_input.at(i).at(j - 1);
                    raw_value += whole_input.at(i).at(j);
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    raw_value = "";
                    prev_index = column + 1;
                    last_inequalities = false;
                    double_equal = false;
                    column++;
                    continue;
                }
                if(raw_value.length() > 0){
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    raw_value = "";
                    prev_index = column;
                }
                if (j + 1 < whole_input.at(i).size()) { // check for <=, >=, !=
                    if (whole_input.at(i).at(j + 1) == '=') {
                        last_inequalities = true;
                        prev_index = column;
                        column++;
                        continue;
                    }
                }
                if (!last_inequalities) { // not equal to included
                    Token* new_token = new Token();
                    new_token->raw_value = whole_input.at(i).at(j);
                    new_token->column = prev_index;
                    new_token->row = row;
                    raw_value = "";
                    tokens.push_back(new_token);
                    prev_index = column + 1;
                }
                last_digit = false;
                variable = false;
            }
            else if(possible_values.count(whole_input.at(i).at(j))){ //operators
                if (j + 1 < whole_input.at(i).size() && whole_input.at(i).at(j) == '=') {
                    if (whole_input.at(i).at(j + 1) == '=') {
                        double_equal = true;
                        ++column;
                        continue;
                    }
                }
                if(raw_value.length() > 0){
                    if(raw_value[raw_value.length()-1] == '.'){
                        cout << "Syntax error on line " << row << " column " << column << "." << endl;
                    }
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    raw_value = "";
                    prev_index = column;
                }
                Token* new_token = new Token();
                new_token->raw_value = whole_input.at(i).at(j);
                new_token->column = prev_index;
                new_token->row = row;
                tokens.push_back(new_token);
                prev_index = column + 1;
                last_digit = false;
                variable = false;
                // change here for variable 
            } else if (isalpha(whole_input.at(i).at(j)) || whole_input.at(i).at(j) == '_' || variable) {
                if (last_digit && !variable) { // variable that starts with number
                    cout << "Syntax error on line " << row << " column " << column << "." << endl;
                    exit(1);
                }
                raw_value += whole_input.at(i).at(j);
                variable = true;
                last_digit = false;
                if (j + 1 < whole_input.at(i).size()) {
                    if (whole_input.at(i).at(j + 1) == ' ' || whole_input.at(i).at(j + 1) == '\t') {
                        variable = false;
                    }
                }
                else if (j + 1 == whole_input.at(i).size()) {
                    variable = false;
                }
            } else if(isdigit(whole_input.at(i).at(j))){
                raw_value += whole_input.at(i).at(j);
                last_digit = true;
            } else if(raw_value.length() > 0){
                if(whole_input.at(i).at(j) == '.'){
                    if (variable) {
                        cout << "Syntax error on line " << row << " column " << column << "." << endl;
                        exit(1);
                    }
                    raw_value += whole_input.at(i).at(j);
                    if(count(raw_value.begin(), raw_value.end(), '.') > 1){ // multiple decimals
                        cout << "Syntax error on line " << row << " column " << column << "." << endl;
                        exit(1);
                    } else if(j == whole_input.at(i).length() -1 || !isdigit(whole_input.at(i).at(j + 1))){
                        cout << "Syntax error on line " << row << " column " << column + 1 << "." << endl;
                        exit(1);
                    }
                } else if(whole_input.at(i).at(j) == ' ' && raw_value.length() == 1){ // ending decimal // ending variable with length 1
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = column - 1;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    raw_value = "";
                    prev_index = column + 1;
                    variable = false;
                    last_digit = false;
                } else{    // ending variable with more than length 1?
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    raw_value = "";
                    prev_index = column + 1;
                    variable = false;
                    last_digit = false;
                }
            } else if(!possible_values.count(whole_input.at(i).at(j)) &&  !isspace(whole_input.at(i).at(j))){ // not a possible token
                cout << "Syntax error on line " << row << " column " << column << "." << endl;
                exit(1);
            } 
            if (isspace(whole_input.at(i).at(j))){
                prev_index = column + 1;
                variable = false;
                last_digit = false;
            }
            column++;
        }
        if (raw_value.length() > 0){
            Token* new_token = new Token();
            new_token->raw_value = raw_value;
            new_token->column = prev_index;
            new_token->row = row;
            tokens.push_back(new_token);
        }
        if (i == whole_input.size() - 1){
            Token* new_token = new Token();
            new_token->raw_value = "END";
            new_token->column = column;
            new_token->row = row;
            tokens.push_back(new_token);
        }
        last_digit = false;
        variable = false;
        raw_value = "";
        column = 1;
        prev_index = 1;
        row++;
    }
}

void Lexer::create_endtokens(){
    string raw_value = "";
    int row = 1; 
    int column = 1;
    int prev_index = 1;
    bool variable = false;
    bool last_digit = false;
    bool last_inequalities = false;
    Token* add_token = nullptr;
    vector<Token*> mini;
    bool error = false;
    bool prev_error = false;

    for(size_t i = 0; i < whole_input.size(); i++){
        for(size_t j = 0; j < whole_input.at(i).length(); j++){
            if (inequalities.count(whole_input.at(i).at(j)) || last_inequalities) {
                if(raw_value.length() > 0 && !last_inequalities){
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    mini.push_back(new_token);
                    raw_value = "";
                    prev_index = column;
                    prev_error = false;
                }
                if (last_inequalities) {
                    raw_value += whole_input.at(i).at(j);
                    add_token = new Token();
                    add_token->raw_value = raw_value;
                    add_token->column = prev_index;
                    add_token->row = row;
                    mini.push_back(add_token);
                    raw_value.clear();
                    prev_index = column + 1;
                    last_digit = false;
                    variable = false;
                    last_inequalities = false;
                    column++;
                    continue;
                }
                raw_value += whole_input.at(i).at(j);
                if (j + 1 < whole_input.at(i).size()) {
                    if (whole_input.at(i).at(j + 1) == '=') {
                        last_inequalities = true;
                        column++;
                        continue;
                    }
                }
                // < > case
                add_token = new Token();
                add_token->raw_value = raw_value;
                add_token->column = prev_index;
                add_token->row = row;
                mini.push_back(add_token);
                raw_value.clear();
                prev_index = column + 1;
                last_digit = false;
                variable = false;
            }
            else if(possible_values.count(whole_input.at(i).at(j))){ //operators
                if(raw_value.length() > 0){
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    mini.push_back(new_token);
                    raw_value = "";
                    prev_index = column;
                }
                // == case
                if (j + 1 < whole_input.at(i).size() && whole_input.at(i).at(j) == '=') {
                    if (whole_input.at(i).at(j + 1) == '=') {
                        raw_value += whole_input.at(i).at(j);
                        last_inequalities = true;
                        ++column;
                        continue;
                    }
                }
                Token* new_token = new Token();
                new_token->raw_value = whole_input.at(i).at(j);
                new_token->column = prev_index;
                new_token->row = row;
                mini.push_back(new_token);
                prev_index = column + 1;
                last_digit = false;
                variable = false;
                // change here for variable 
            } else if (isalpha(whole_input.at(i).at(j)) || whole_input.at(i).at(j) == '_' || variable) {
                if (last_digit && !variable) { // variable that starts with number
                    cout << "exit here 5";
                    cout << "Syntax error on line " << row << " column " << column << "." << endl;
                    error = true;
                }
                raw_value += whole_input.at(i).at(j);
                variable = true;
                last_digit = false;
                if (j + 1 < whole_input.at(i).size()) {
                    if (whole_input.at(i).at(j + 1) == ' ' || whole_input.at(i).at(j + 1) == '\t') {
                        variable = false;
                    }
                }
                else if (j + 1 == whole_input.at(i).size()) {
                    variable = false;
                }
            } else if(isdigit(whole_input.at(i).at(j))){
                raw_value += whole_input.at(i).at(j);
                last_digit = true;
            } else if(raw_value.length() > 0){
                if(whole_input.at(i).at(j) == '.'){
                    if (variable) {
                        cout << "Syntax error on line " << row << " column " << column << "." << endl;
                        error = true;
                    }
                    raw_value += whole_input.at(i).at(j);
                    if(count(raw_value.begin(), raw_value.end(), '.') > 1){ // multiple decimals
                        cout << "Syntax error on line " << row << " column " << column << "." << endl;
                        error = true;
                    } else if((j == whole_input.at(i).length() -1 || !isdigit(whole_input.at(i).at(j + 1)))){
                        cout << "Syntax error on line " << row << " column " << column + 1 << "." << endl;
                        prev_error = true;
                        error = true;
                    }
                } else if(whole_input.at(i).at(j) == ' ' && raw_value.length() == 1){ // ending decimal // ending variable with length 1
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->row = row;
                    new_token->column = column - 1;
                    mini.push_back(new_token);
                    raw_value = "";
                    prev_index = column + 1;
                    variable = false;
                    last_digit = false;
                } else{    // ending variable with more than length 1?
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    mini.push_back(new_token);
                    raw_value = "";
                    prev_index = column + 1;
                    variable = false;
                    last_digit = false;
                }
            } else if((!possible_values.count(whole_input.at(i).at(j)) &&  !isspace(whole_input.at(i).at(j))) && !prev_error){ // not a possible token
                cout << "Syntax error on line " << row << " column " << column << "." << endl;
                error = true;
                this->error = true;
                prev_error = true;
            } 
            if (isspace(whole_input.at(i).at(j))){
                prev_index = column + 1;
                variable = false;
                last_digit = false;
                prev_error = false;
            }
            column++;
        }
        prev_error = false;
        if (raw_value.length() > 0){
            Token* new_token = new Token();
            new_token->raw_value = raw_value;
            new_token->column = prev_index;
            new_token->row = row;
            mini.push_back(new_token);
        }
        
        // cout << mini.size() << endl;
        if(mini.size() > 0 && mini.at(mini.size() - 1)->raw_value != "END" ){
            Token* new_token = new Token();
            new_token->raw_value = "END";
            new_token->column = column;
            new_token->row = row;
            mini.push_back(new_token);
        } else if (mini.size() == 0 && multi_end_tokens.size() == 0){
            Token* new_token = new Token();
            new_token->raw_value = "END";
            new_token->column = column;
            new_token->row = row;
            mini.push_back(new_token);
        } else if (mini.size() == 0 && (multi_end_tokens.at(multi_end_tokens.size() - 1)->raw_value != "END")){
            Token* new_token = new Token();
            new_token->raw_value = "END";
            new_token->column = column;
            new_token->row = row;
            mini.push_back(new_token);
        }

        // Token* new_token = new Token();
        //     new_token->raw_value = "END";
        //     new_token->column = column;
        //     new_token->row = row;
        //     mini.push_back(new_token);
        if (!error) {
            for (auto a: mini) {
                multi_end_tokens.push_back(a);
            }
        }
        else {
            for (auto a: mini) {
                delete a;
            }
        }
        error = false;
        mini.clear();
        
        last_digit = false;
        variable = false;
        raw_value = "";
        column = 1;
        prev_index = 1;
    }
}

void Lexer::print_endtokens(){
    for(size_t i = 0; i < multi_end_tokens.size(); i++){
        cout << setw(4) << right << multi_end_tokens.at(i)->row;
        cout << setw(5) << right << multi_end_tokens.at(i)->column;
        cout << "  " << multi_end_tokens.at(i)->raw_value << endl;
    }
}
void Lexer::print_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        cout << setw(4) << right << tokens.at(i)->row;
        cout << setw(5) << right << tokens.at(i)->column;
        cout << "  " << tokens.at(i)->raw_value << endl;
    }
}

void Lexer::delete_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        delete tokens.at(i);
    }
}

void Lexer::delete_endtokens(){
    for(size_t i = 0; i < multi_end_tokens.size(); i++){
        delete multi_end_tokens.at(i);
    }
}