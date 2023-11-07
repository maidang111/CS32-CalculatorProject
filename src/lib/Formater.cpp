
#include <iostream>
#include <vector>
#include <string>
#include "Formater.h"
using namespace std;

Formater::Formater(vector<Token*> tokens){
    this->tokens = tokens;
    this->index = 0;
}

Formater::~Formater(){
}

void Formater::deleteStatements(){
    for(size_t i = 0; i < ASTHeads.size(); i++){
            // cout << i;
            // cout << ASTHeads.at(i)->body.size();
            ASTHeads.at(i)->deleteStatement();
        }
    delete_tokens();
}
void Formater::buildASTs(){
    while(index != tokens.size()){
        if(tokens.at(index)->raw_value == "END"){
            index++;
        } else {
            Statement* root = buildAST();
            // root->print();
            ASTHeads.push_back(root);
            // ASTHeads.at(0)->print();
        }
    }
}

Statement* Formater::buildAST(){
    if(tokens.at(index)->raw_value == "while"){
        index++;
        While* whileBlock = new While();
        while(tokens.at(index)->raw_value != "{"){
            whileBlock->condition.push_back(tokens.at(index));
            index++;
        }
        index++;
        whileBlock->condition.push_back(tokens.at(index));
        index++;
        
        while(tokens.at(index)->raw_value != "}"){
            // cout << tokens.at(index)->raw_value;
            whileBlock->body.push_back(buildAST());
        }
        index++;
        return whileBlock;
    } else if (tokens.at(index)->raw_value == "if"){
        index++;
        If* ifBlock = new If();
        while(tokens.at(index)->raw_value != "{"){
            ifBlock->condition.push_back(tokens.at(index));
            index++;
        }
        index++;
        ifBlock->condition.push_back(tokens.at(index));
        index++;
        while(tokens.at(index)->raw_value != "}"){
            // cout << tokens.at(index)->raw_value;
            ifBlock->body.push_back(buildAST());
        }
        index++;
        return ifBlock;
    } else if (tokens.at(index)->raw_value == "else"){
        index++;
        Else* elseBlock = new Else();
        while(tokens.at(index)->raw_value != "{"){
            elseBlock->condition.push_back(tokens.at(index));
            index++;
        }
        index++;
        elseBlock->condition.push_back(tokens.at(index));
        index++;
        
        while(tokens.at(index)->raw_value != "}"){
            // cout << tokens.at(index)->raw_value;
            elseBlock->body.push_back(buildAST());
        }
        index++;
        return elseBlock;
    } else if (tokens.at(index)->raw_value == "print"){
        index++;
        Print* printBlock = new Print();
        printBlock->body.push_back(buildAST());
        // cout << printBlock->body.size();
        return printBlock;
    } else {
        Expression* expressionBlock = new Expression();
        while (tokens.at(index)->raw_value != "END"){
            expressionBlock->body.push_back(tokens.at(index));
            index++;
        }
        expressionBlock->body.push_back(tokens.at(index));
        index++;
        return expressionBlock;
    }
}

void Formater::printFormated(){
    for(size_t i = 0; i < ASTHeads.size(); i++){
        // cout << i;
        // cout << ASTHeads.at(i)->body.size();
        ASTHeads.at(i)->print();
    }
}
void Formater::deleteFunc(){
    // cout << ASTHeads.size() << endl;

    for(size_t i = 0 ; i < ASTHeads.size(); i++){
        // cout << "deleteFuc" << i << endl;
        // cout << ASTHeads.at(i)->body.size() << endl;
        delete_help(ASTHeads.at(i));
    }
}

void Formater::check() {
    cout << ASTHeads.size() << endl;
}

void Formater::delete_help(Statement* node) {
    // cout << "1" << endl;
    if (!node) {
        return;
    }
    // cout << "2" << endl;
    for (size_t i = 0; i < node->body.size(); ++i) {
        delete_help(node->body.at(i));
    }
    // cout << "3" << endl;
    delete node; 
}

void Formater::delete_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        delete tokens.at(i);
    }   
}