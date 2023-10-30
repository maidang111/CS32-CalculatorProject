#include <iostream>
#include <string>
#include <vector>
#include "InfixParser.h"
#include "Node.h"
#include "Lexer.h"
#include "AST.h"

using namespace std; 

InfixParser::InfixParser(){
    Lexer lexer; 
    lexer.create_endtokens();
    this->tokens = lexer.multi_end_tokens;
    this->count = 0;
}
InfixParser::~InfixParser(){}

void InfixParser::build_AST(){
    while(count != tokens.size()){
        scanToken();
        if (nextToken->raw_value != "END"){
            AST = parseExpression();
            if (nextToken->raw_value != "END"){
            cout << "next token isn't END token: " << nextToken->raw_value;
            exit(1) ;
            } 
            AST->print();
            cout << endl;
            double value = AST->get_value();
            cout << value;
            for(size_t i = 0; i < variables.size(); i++){
                if (variables.at(i)->value == 0.0){
                    variables.at(i)->value = value;
                }
            }
            cout << endl;
            AST->deleteToken();
        } 
    }
//    AST->get_value();
}

void InfixParser::scanToken(){
    if (count != tokens.size()){
        this->nextToken = tokens.at(count);
        count++;
    }
}

Token* InfixParser::parseExpression(){
    Token* term = parseTerm();
    while (true){
        if (nextToken == nullptr){
            cout << "null expression" << endl;
            exit(1);
        } else if(nextToken->raw_value == "+"){
            scanToken();
            Token* term1 = parseTerm();
            Add* temp = new Add;
            temp->left = term;
            temp->right = term1;
            term = temp;
        } else if(nextToken->raw_value == "-"){
            scanToken();
            Token* term1 = parseTerm();
            Subtract* temp = new Subtract;
            temp->left = term;
            temp->right = term1;
            term = temp;
        } else {
            return term;
        }
    }
}

Token* InfixParser::parseTerm(){
    Token* factor = parseFactor();
    while (true){
        if (nextToken == nullptr){
            cout << "null expression" << endl;
            exit(1);
        } else if(nextToken->raw_value == "*"){
            scanToken();
            Token* factor1 = parseFactor();
            Multiply* temp = new Multiply;
            temp->left = factor;
            temp->right = factor1;
            factor = temp;
        } else if(nextToken->raw_value == "/"){
            scanToken();
            Token* factor1 = parseFactor();
            Divide* temp = new Divide;
            temp->left = factor;
            temp->right = factor1;
            factor = temp;
        } else if(nextToken->raw_value == "="){
            scanToken();
            Token* factor1 = parseTerm();
            Equal* temp = new Equal;
            temp->left = factor;
            temp->right = factor1;
            factor = temp;
        } else {
            return factor;
        }
    }
}

Token* InfixParser::parseFactor(){
    if(isdigit(nextToken->raw_value[0])){
        Num* num = new Num;
        num->raw_value = nextToken->raw_value;
        scanToken();
        return num;
    } else if (isalpha(nextToken->raw_value[0])){
        Variable* variable = new Variable;
        Variable* variable2 = new Variable;
        for(size_t i = 0; i < variables.size(); i++){
            if(nextToken->raw_value == variables.at(i)->raw_value){
                variable->raw_value = variables.at(i)->raw_value;
                variable->value = variables.at(i)->value;
                scanToken();
                return variable;
            }
        }
        variables.push_back(variable2);
        variable2->raw_value = nextToken->raw_value;
        variable->raw_value = nextToken->raw_value;
        scanToken();
        return variable;
    } else if (nextToken->raw_value == "("){
        scanToken();
        Token* expression = parseExpression();
        if (expression == nullptr) {
            return nullptr;
        }
        if(nextToken->raw_value == ")"){
            scanToken();
            return expression;
        } else {
            return nullptr;
        }
    } else {
        cout << "Invalid factor " << nextToken->raw_value << endl;
        exit(1);
    }
}

void InfixParser::delete_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        delete tokens.at(i);
    }
}

void InfixParser::delete_variables(){
    for(size_t i = 0; i < variables.size(); i++){
        delete variables.at(i);
    }
}
