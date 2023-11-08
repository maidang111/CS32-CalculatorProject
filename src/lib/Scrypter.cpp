#include <iostream>
#include "Scrypter.h"
using namespace std;

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
        // while(tokens.at(index)->raw_value == "END"){
        //     index++;
        // }
        while(tokens.at(index)->raw_value != "}"){
            // cout << level << endl;
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
            // cout << tokens.at(index)->raw_value;
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
        // cout << level << "elseBlock" << endl;
        while(tokens.at(index)->raw_value != "{"){
            elseBlock->condition.push_back(tokens.at(index));
            index++;
        }
        index++;
        elseBlock->condition.push_back(tokens.at(index));
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
        // cout << printBlock->body.size();
        return printBlock;
    } else {
        Expression* expressionBlock = new Expression();
        expressionBlock->level = level;
        // cout << "expressionblock" << level << endl;
        while (tokens.at(index)->raw_value != "END"){
            expressionBlock->body.push_back(tokens.at(index));
            index++;
        }
        expressionBlock->body.push_back(tokens.at(index));
        index++;
        return expressionBlock;
    }
}
