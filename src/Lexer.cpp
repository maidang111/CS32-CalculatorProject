#include <iostream>
#include <string>
#include <set>
#include "lib/Lexer.h"
#include "Token.cpp"

using namespace std; 

set<char>possible_values = {'(', ')', '+', '-', '*', '/'};

Lexer::Lexer(string text){
    this->text = text;
}

void Lexer::create_tokens(){
    for(char& c: text ){
        cout << c << endl;
    }
}

void Lexer::print_tokens(){
    
}