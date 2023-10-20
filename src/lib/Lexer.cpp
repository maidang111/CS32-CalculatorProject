
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
    // possible token values excluding the END

    string line = "";
    while (!cin.eof()){ 
        //gets user input and stores it in a vector as a series of strings

        getline(cin, line);
        this->whole_input.push_back(line);
    }
}

Lexer::~Lexer(){}

void Lexer::create_tokens(){
    string value = "";
    int row = 1; 
    int column = 1;
    int prev_index = 1;

    for(size_t i = 0; i < whole_input.size(); i++){ 
        // looping through each string and it's char from user input

        for(size_t j = 0; j < whole_input.at(i).length(); j++){
            if(possible_values.count(whole_input.at(i).at(j))){ 
                // checks if char is a valid token type

                if(value.length() > 0){
                    // creates token for a number
                
                    Token* new_token = new Token();
                    new_token->value = value;
                    new_token->column = prev_index;
                    new_token->row = row;
                    tokens.push_back(new_token);
                    value = "";
                    prev_index = column;
                }
                // creates a token for a valid token type that isn't a number

                Token* new_token = new Token();
                new_token->value = whole_input.at(i).at(j);
                new_token->column = prev_index;
                new_token->row = row;
                tokens.push_back(new_token);
                prev_index = column + 1;
            } else if(isdigit(whole_input.at(i).at(j))){
                // checking if a char is a number and adding it to a string

                value += whole_input.at(i).at(j);
            } else if(value.length() > 0){
                // checking if a string storing numbers is empty

                if(whole_input.at(i).at(j) == '.'){
                    value += whole_input.at(i).at(j);
                    if(count(value.begin(), value.end(), '.') > 1){ 
                        // multiple decimals errors out
                        cout << "Syntax error on line " << row << " column " << column << "." << endl;
                        exit(1);

                    } else if(j == whole_input.at(i).length() -1 || !isdigit(whole_input.at(i).at(j + 1))){
                        // if number ends w/ a decimal

                        cout << "Syntax error on line " << row << " column " << column + 1 << "." << endl;
                        exit(1);
                    }
                } else if(whole_input.at(i).at(j) == ' ' && value.length() == 1){ 
                    // if it's a number w/ len 1 followed by a space

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
            } else if(!possible_values.count(whole_input.at(i).at(j)) &&  !isspace(whole_input.at(i).at(j))){ 
                // not a possible token

                cout << "Syntax error on line " << row << " column " << column << "." << endl;
                exit(1);
            } 
            if (isspace(whole_input.at(i).at(j))){
                prev_index = column + 1;
            }
            column++;
        }
        // adding last token of a string if any
        if (value.length() > 0){
            Token* new_token = new Token();
            new_token->value = value;
            new_token->column = prev_index;
            new_token->row = row;
            tokens.push_back(new_token);
        }
        // adding END token to vector
        if (i == whole_input.size() - 1){
            Token* new_token = new Token();
            new_token->value = "END";
            new_token->column = column;
            new_token->row = row;
            tokens.push_back(new_token);
        }
        value = "";
        column = 1;
        prev_index = 1;
        row++;
    }
}

void Lexer::print_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        cout << setw(4) << right << tokens.at(i)->row;
        cout << setw(5) << right << tokens.at(i)->column;
        cout << "  " << tokens.at(i)->value << endl;
    }
}

void Lexer::delete_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        delete tokens.at(i);
    }
}