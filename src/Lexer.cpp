#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <iomanip>
#include "lib/Lexer.h"
#include <algorithm>

using namespace std; 

Lexer::Lexer(){
    this->possible_values = {'(', ')', '+', '-', '*', '/', 'E'};
    string line;
    while (line != "END"){
        getline(cin, line);
        this->whole_input.push_back(line);
    }
}

Lexer::~Lexer(){}

void Lexer::create_tokens(){
    string value = "";
    size_t row = 1; 
    size_t column = 1;
    size_t prev_index = 1;

    for(size_t i = 0; i < whole_input.size(); i++){
        for(size_t j = 0; j < whole_input.at(i).length(); j++){
            if(possible_values.count(whole_input.at(i).at(j))){
                if(value.length() > 0){
                    cout << value << endl;
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
            } else if(value.length() > 0){
                cout << value.length() << endl;
                cout << "[" << value << "]" << endl;
                if(whole_input.at(i).at(j) == '.'){
                    value += whole_input.at(i).at(j);
                }else{
                    Token* new_token = new Token();
                    new_token->value = value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    value = "";
                    prev_index = column + 1;
                }
            } else if(isdigit(whole_input.at(i).at(j))){
                cout << value.length() << endl;
                cout << "[" << value << "]" << endl;
                value += whole_input.at(i).at(j);
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
    Token* last_token = tokens.back();
    last_token->value = "END";
}

void Lexer::print_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        cout << setw(4) << left << tokens.at(i)->row;
        cout << setw(4) << right << tokens.at(i)->column;
        cout << "   " << setw(4) << left << tokens.at(i)->value << endl;
    }
}
