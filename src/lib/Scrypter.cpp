
#include <iostream>
#include <vector>
#include <string>
#include "Scrypter.h"
using namespace std;

Scrypter::Scrypter(vector<Token*> tokens){
    InfixParser* infixparser = new InfixParser();
    this->infixparser = infixparser;
    this->tokens = tokens;
    this->index = 0;
}

Scrypter::~Scrypter(){
    delete infixparser;
}

void Scrypter::deleteStatements(){
    for(size_t i = 0; i < ASTHeads.size(); i++){
            // cout << i;
            // cout << ASTHeads.at(i)->body.size();
            ASTHeads.at(i)->deleteStatement();
        }
    delete_tokens();
}

void Scrypter::buildASTs(){
    while(index != tokens.size()){
        if(tokens.at(index)->raw_value == "END"){
            index++;
        } else {
            level = 0;
            Statement* root = buildAST();
            // root->print();
            ASTHeads.push_back(root);
            // ASTHeads.at(0)->print();
        }
    }
}

Statement* Scrypter::buildAST(){
    if(tokens.at(index)->raw_value == "while"){
        index++;
        While* whileBlock = new While();
        whileBlock->level = level;
        level++;
        while(tokens.at(index)->raw_value != "{"){
            whileBlock->condition.push_back(tokens.at(index));
            index++;
        }
        index++;
        whileBlock->condition.push_back(tokens.at(index));
        index++;
        
        while(tokens.at(index)->raw_value != "}"){
            if(tokens.at(index)->raw_value != "END"){
                size_t tempLevel = level;
                whileBlock->body.push_back(buildAST());
                level = tempLevel;
            } else {
                index++;
            }
        }
        index++;
        return whileBlock;
    } else if (tokens.at(index)->raw_value == "if"){
        index++;
        If* ifBlock = new If();
        ifBlock->level = level;
        level++;
        while(tokens.at(index)->raw_value != "{"){
            ifBlock->condition.push_back(tokens.at(index));
            index++;
        }
        index++;
        ifBlock->condition.push_back(tokens.at(index));
        index++;
        while(tokens.at(index)->raw_value != "}"){
            if(tokens.at(index)->raw_value != "END"){
                size_t tempLevel = level;
                ifBlock->body.push_back(buildAST());
                level = tempLevel;
            } else {
                index++;
            }
        }
        index++;
        return ifBlock;
    } else if (tokens.at(index)->raw_value == "else"){
        index++;
        Else* elseBlock = new Else();
        elseBlock->level = level;
        level++;
        if (tokens.at(index)->raw_value == "if"){
            // size_t tempLevel = level;
            // elseBlock->body.push_back(buildAST());
            // level = tempLevel;
            // index++;
            // elseBlock->body.push_back(buildAST());
            // return elseBlock;
            size_t tempLevel = level;
            elseBlock->body.push_back(buildAST());
            level = tempLevel;
            index++;
            if (tokens.at(index)->raw_value == "else"){
                Statement* elseBody = buildAST();
                elseBlock->body.push_back(elseBody);
            }
            index -= 2;
            return elseBlock;
        }
        index++;
        while(tokens.at(index)->raw_value != "}"){
            if(tokens.at(index)->raw_value != "END"){
                size_t tempLevel = level;
                elseBlock->body.push_back(buildAST());
                level = tempLevel;
            } else {
                index++;
            }
        }
        index++;
        return elseBlock;
    } else if (tokens.at(index)->raw_value == "print"){
        index++;
        Print* printBlock = new Print();
        printBlock->level = level;
        size_t tempLevel = level;
        level = 0;
        printBlock->body.push_back(buildAST());
        level = tempLevel;
        return printBlock;
    } else {
        if(tokens.at(index)->raw_value == "}"){
            index += 2;
            // if(tokens.at(index)->raw_value == "END"){
            return nullptr;
        }
        Expression* expressionBlock = new Expression();
        expressionBlock->level = level;
        while (tokens.at(index)->raw_value != "END"){
            expressionBlock->body.push_back(tokens.at(index));
            index++;
        }
        expressionBlock->body.push_back(tokens.at(index));
        index++;
        return expressionBlock;
    }
}

void Scrypter::calculate(){
    for(size_t i = 0; i < ASTHeads.size(); i++){
        ASTHeads.at(i)->calculate(infixparser);
    }
}
void Scrypter::deleteFunc(){
    for(size_t i = 0 ; i < ASTHeads.size(); i++){
        delete_help(ASTHeads.at(i));
    }
}

void Scrypter::check() {
    cout << ASTHeads.size() << endl;
}

void Scrypter::delete_help(Statement* node) {
    if (!node) {
        return;
    }
    for (size_t i = 0; i < node->body.size(); ++i) {
        delete_help(node->body.at(i));
    }
    delete node; 
}

void Scrypter::delete_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        delete tokens.at(i);
    }   
}