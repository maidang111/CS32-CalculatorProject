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
    // cout << "destructor" << endl;
    // cout << ASTHeads.size();
    deleteFunc();
}

void Formater::buildASTs(){
    while(index != tokens.size()){
        if(tokens.at(index)->raw_value == "END"){
            index++;
        } else {
            Statement* root = buildAST();
            ASTHeads.push_back(root);
        }
    }
}

Statement* Formater::buildAST(){
    if(tokens.at(index)->raw_value == "while"){
        index++;
        if(tokens.at(index)->raw_value == "END"){
            return nullptr;
        }
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
        if(tokens.at(index)->raw_value == "END"){
            return nullptr;
        }
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
        if(tokens.at(index)->raw_value == "END"){
            return nullptr;
        }
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
        if(tokens.at(index)->raw_value == "END"){
            return nullptr;
        }
        Print* printBlock = new Print();
        printBlock->body.push_back(buildAST());
        return printBlock;
    } else {
        if(tokens.at(index)->raw_value == "END"){
            return nullptr;
        }
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
        ASTHeads.at(i)->print();
    }
}
void Formater::deleteFunc(){
    for(size_t i = 0 ; i < ASTHeads.size(); i++){
        // cout << "deleteFuc" << i << endl;
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
        node->body.at(i)->print();
        cout << endl;
        delete_help(node->body.at(i));
    }
    // cout << "3" << endl;
    delete node; 
}

