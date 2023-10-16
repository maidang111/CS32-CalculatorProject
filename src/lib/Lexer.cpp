#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <iomanip>
#include "Lexer.h"
#include <algorithm>

using namespace std; 

Lexer::Lexer(){
    this->possible_values = {'(', ')', '+', '-', '*', '/'};
    string line = "";
    char n;
    while (!cin.eof()){
        cin.get(n);
        if (n == '\n'){
            whole_input.push_back(line);
            line = "";
        } else {
            line += n;
        }
    }
}

Lexer::~Lexer(){
    for(size_t i = 0; i < tokens.size(); i++){
        delete tokens.at(i);
    }
}

void Lexer::create_tokens(){
    string value = "";
    size_t row = 1; 
    size_t column = 1;
    size_t prev_index = 1;

    for (size_t i = 0; i < whole_input.size(); i++){
        cout << i << endl;
    }

    for(size_t i = 0; i < whole_input.size(); i++){
        for(size_t j = 0; j < whole_input.at(i).length(); j++){
            if (whole_input.at(i).at(j) == '\t'){
                column += 5;
            }
            if(possible_values.count(whole_input.at(i).at(j))){
                if(value.length() > 0){
                    Token* new_token = new Token();
                    new_token->value = value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    value = "";
                    prev_index = column;
                }
                Token* new_token = new Token();
                new_token->value = whole_input.at(i).at(j);
                new_token->column = prev_index;
                new_token->row = row;
                tokens.push_back(new_token);
                prev_index = column + 1;
            } else if(isdigit(whole_input.at(i).at(j))){
                value += whole_input.at(i).at(j);
            } else if(value.length() > 0){
                if(whole_input.at(i).at(j) == '.'){
                    value += whole_input.at(i).at(j);
                    if(count(value.begin(), value.end(), '.') > 1){ // multiple decimals
                        cout << "Syntax error on line " << row << " column " << column << "." << endl;
                        exit(1);
                    } else if(j == whole_input.at(i).length() -1 || !isdigit(whole_input.at(i).at(j + 1))){
                        cout << "Syntax error on line " << row << " column " << column + 1 << "." << endl;
                        exit(1);
                    }
                } else if(whole_input.at(i).at(j) == ' ' && value.length() == 1){ // ending decimal
                    Token* new_token = new Token();
                    new_token->value = value;
                    new_token->column = column - 1;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    value = "";
                    prev_index = column + 1;
                } else{
                    Token* new_token = new Token();
                    new_token->value = value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    value = "";
                    prev_index = column + 1;
                }
            } else if(!possible_values.count(whole_input.at(i).at(j)) && !isspace(whole_input.at(i).at(j))){ // not a possible token
                cout << "Syntax error on line " << row << " column " << column << "." << endl;
                exit(1);
            }
            column++;
        }
        if (value.length() > 0){
            Token* new_token = new Token();
            new_token->value = value;
            new_token->column = prev_index;
            new_token->row = row;
            tokens.push_back(new_token);
        }
        value = "";
        column = 1;
        prev_index = 1;
        row++;
    }
    Token* new_token = new Token();
            new_token->value = "END";
            new_token->column = prev_index;
            new_token->row = row -1;
            tokens.push_back(new_token);
}

void Lexer::print_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        cout << setw(4) << right << tokens.at(i)->row;
        cout << setw(5) << right << tokens.at(i)->column;
        cout << "  " << tokens.at(i)->value << endl;
    }
}
