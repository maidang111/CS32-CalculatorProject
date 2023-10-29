
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
    this->possible_values = {'=', '(', ')', '+', '-', '*', '/'};
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

Lexer::~Lexer(){}

void Lexer::create_tokens(){
    string raw_value = "";
    int row = 1; 
    int column = 1;
    int prev_index = 1;
    bool variable = false;
    bool last_digit = false;

    for(size_t i = 0; i < whole_input.size(); i++){
        for(size_t j = 0; j < whole_input.at(i).length(); j++){
            if(possible_values.count(whole_input.at(i).at(j))){ //operators
                if(raw_value.length() > 0){
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

    raw_value = "";
    row = 1; 
    column = 1;
    prev_index = 1;
    variable = false;
    last_digit = false;

    for(size_t i = 0; i < whole_input.size(); i++){
        for(size_t j = 0; j < whole_input.at(i).length(); j++){
            if(possible_values.count(whole_input.at(i).at(j))){ //operators
                if(raw_value.length() > 0){
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    multi_end_tokens.push_back(new_token);
                    raw_value = "";
                    prev_index = column;
                }
                Token* new_token = new Token();
                new_token->raw_value = whole_input.at(i).at(j);
                new_token->column = prev_index;
                new_token->row = row;
                multi_end_tokens.push_back(new_token);
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
                    multi_end_tokens.push_back(new_token);
                    raw_value = "";
                    prev_index = column + 1;
                    variable = false;
                    last_digit = false;
                } else{    // ending variable with more than length 1?
                    Token* new_token = new Token();
                    new_token->raw_value = raw_value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    multi_end_tokens.push_back(new_token);
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
            multi_end_tokens.push_back(new_token);
        }
        
        if (multi_end_tokens.size() > 1 && (multi_end_tokens.at(multi_end_tokens.size()-2)->raw_value != "END")){
            Token* new_token = new Token();
            new_token->raw_value = "END";
            new_token->column = column;
            new_token->row = row;
            multi_end_tokens.push_back(new_token);
        }

        last_digit = false;
        variable = false;
        raw_value = "";
        column = 1;
        prev_index = 1;
        row++;
    }

    if(multi_end_tokens.at(multi_end_tokens.size() - 1)->value == multi_end_tokens.at(multi_end_tokens.size() - 2)->value){
        multi_end_tokens.pop_back();
    }

}

void Lexer::print_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        cout << setw(4) << right << tokens.at(i)->row;
        cout << setw(5) << right << tokens.at(i)->column;
        cout << "  " << tokens.at(i)->raw_value << endl;
    }
}

void Lexer::print_endtokens(){
    for(size_t i = 0; i < multi_end_tokens.size(); i++){
        cout << setw(4) << right << multi_end_tokens.at(i)->row;
        cout << setw(5) << right << multi_end_tokens.at(i)->column;
        cout << "  " << multi_end_tokens.at(i)->raw_value << endl;
    }
}

void Lexer::delete_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        delete tokens.at(i);
    }
}