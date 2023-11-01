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
            AST = parseEqual();
            if (nextToken->raw_value != "END" || AST == nullptr){
                cout << "Unexpected token at line 1" << " column " << nextToken->column << " " << nextToken->raw_value << endl;
                scanToken();
            } else {
                AST->print();
                cout << endl;
                cout << AST->get_value() << endl;
                ASTheads.push_back(AST);
            }
        } 
    }
    for(size_t i = 0; i < ASTheads.size(); i++){
        ASTheads.at(i)->deleteToken();
    }
//    AST->get_value();
}

void InfixParser::scanToken(){
    if (count != tokens.size()){
        this->nextToken = tokens.at(count);
        count++;
    }
}

void InfixParser::prevToken(){
    if (count != 0){
        count--;
        this->nextToken = tokens.at(count);
    }
}

Token* InfixParser::parseEqual(){
    Token* equal = parseExpression();
    while (true){
        if (nextToken == nullptr){
            cout << "null expression" << endl;
            exit(1);
        } else if(nextToken->raw_value == "="){
            scanToken();
            Token* equal1 = parseEqual();
            Equal* temp = new Equal;
            temp->left = equal;
            temp->right = equal1;
            temp->left->value = equal1->get_value();
            for(size_t i = 0; i < variables.size(); i++){
                if(variables.at(i)->raw_value == temp->left->raw_value){
                    variables.erase(variables.begin()+i);
                }
            }
            variables.push_back(temp->left);      
            equal = temp;
        } else {
            return equal;
        }
    }
}
// for(size_t i = 0; i < variables.size(); i++){
//                 if(variables.at(i)->raw_value == temp->left->raw_value){
//                     variables.at(i)->value = temp->left->value;
//                 }else {
//                     variables.push_back(temp->left);      
//                 }
//             }
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
        for(size_t i = 0; i < variables.size(); i++){
            if(nextToken->raw_value == variables.at(i)->raw_value){
                variable->raw_value = variables.at(i)->raw_value;
                variable->value = variables.at(i)->value;
                scanToken();
                return variable;
            }
        }
        variable->raw_value = nextToken->raw_value;
        scanToken();
        return variable;
    } else if (nextToken->raw_value == "("){
        scanToken();
        Token* expression = parseEqual();
        if (expression == nullptr) {
            cout << "Unexpected token at line 1" << " column " << nextToken->column << " " << nextToken->raw_value << endl;
            AST = nullptr;
            return nullptr;
        }
        if(nextToken->raw_value == ")"){
            scanToken();
            return expression;
        } else {
            cout << "Unexpected token at line 1" << " column " << nextToken->column << " " << nextToken->raw_value << endl;
            AST = nullptr;
            return nullptr;
        }
    } else {
            cout << "Unexpected token at line 1" << " column " << nextToken->column << " " << nextToken->raw_value << endl;
        AST = nullptr;
        return nullptr;
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
