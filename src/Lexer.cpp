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

    for(size_t i = 0; i < whole_input.size(); i++){
        for(size_t j = 0; j < whole_input.at(i).length(); j++){
            if(possible_values.count(whole_input.at(i).at(j))){
                if(value.length() > 0){
                    Token* new_token = new Token();
                    new_token->value = value;
                    new_token->column = column;
                    new_token->row = row;
                    tokens.push_back(new_token);
                }
                Token* new_token = new Token();
                new_token->value = whole_input.at(i).at(j);
                new_token->column = column;
                new_token->row = row;
                tokens.push_back(new_token);
            } else if(isdigit(whole_input.at(i).at(j))){
                value += whole_input.at(i).at(j);
            } else if(whole_input.at(i).at(j) == '.' && value.length() > 0){
                value += whole_input.at(i).at(j);
            }
            column++;
        }
        value = "";
        column = 1;
        row++;
    }
}

void Lexer::print_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        cout << setw(4) << left << tokens.at(i)->row;
        cout << setw(4) << left << tokens.at(i)->column;
        cout << setw(4) << right << tokens.at(i)->value << endl;
    }
}
