
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
    size_t count = 0;
    while(index != tokens.size()){
        if(count == 2){
            cout << "Runtime error: not a function." << endl;
            exit(1);
        }
        if(tokens.at(index)->raw_value == "END"){
            index++;
        } else {
            if(tokens.at(index)->is_function){
                level = 0;
                if (tokens.at(index)->raw_value == "foo"){
                    count++;
                }
                FunctionCall* root = buildFunction();
                if (root != nullptr){
                    ASTHeads.push_back(root);
                }
            } else {
                if (tokens.at(index)->raw_value == "foo"){
                    count++;
                }
                level = 0;
                Statement* root = buildAST();
                if (root != nullptr){
                    ASTHeads.push_back(root);
                }
            }
        }
    }
}

FunctionCall* Scrypter::buildFunction(){
    FunctionCall* functionCall = new FunctionCall();
    // functionCall->is_fuction_def = false;
    functionCall->functionName = tokens.at(index)->raw_value;
    // cout << tokens.at(index)->raw_value << endl;
    index += 2;
    while(tokens.at(index)->raw_value != ")"){
    if (tokens.at(index)->raw_value != ","){
        // cout << tokens.at(index)->raw_value << endl;
        functionCall->parameters.push_back(tokens.at(index));
    }
    index++;
    }
    for(size_t i = 0; i < ASTFunctions.size(); i++){
        if (ASTFunctions.at(i)->functionName == functionCall->functionName){
            functionCall->function = ASTFunctions.at(i);
        }
    }
    return functionCall;
}

Statement* Scrypter::buildAST(){
    if(tokens.at(index)->raw_value == "def"){
        index++;
    Function* function = new Function();
    function->is_fuction_def = true;
    function->level = level;
    function->functionName = tokens.at(index)->raw_value;
    infixparser->functionNames.push_back(tokens.at(index));
    ASTFunctions.push_back(function);
    level++;
    //get function parameters
    index += 2;
    // cout << tokens.at(index)->raw_value << endl << endl;
    while(tokens.at(index)->raw_value == "END"){
        index++;
    }
    while(tokens.at(index)->raw_value != ")"){
        // cout << tokens.at(index)->raw_value << endl;
        if (tokens.at(index)->raw_value != ","){
            function->parameters.push_back(tokens.at(index));
        }
        index++;
    }
    while(tokens.at(index)->raw_value != "{"){
        index++;
    }
    index++;
    // cout << tokens.at(index)->raw_value << endl;
    while(tokens.at(index)->raw_value != "return" && tokens.at(index)->raw_value != "}"){
        if(tokens.at(index)->raw_value != "END"){
                    // cout << tokens.at(index)->raw_value << endl;
                size_t tempLevel = level;
                function->body.push_back(buildAST());
                level = tempLevel;
            } else {
                index++;
            }
    }
    if(tokens.at(index)->raw_value == "}"){
        index += 2;
    }

    // cout << function->functionName << endl;
    //Creating return statement
    if(tokens.at(index)->raw_value == "return"){
        while (tokens.at(index)->raw_value != ";"){
            function->returnStatement.push_back(tokens.at(index));
            index++;
        }
        Token* endToken = new Token;
        endToken->raw_value = "END";
        function->returnStatement.push_back(endToken);
        tokens.push_back(endToken);
        index++;
    }
    return function;
    } else if(tokens.at(index)->raw_value == "while"){
        index++;
        While* whileBlock = new While();
        whileBlock->is_fuction_def = false;
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
        ifBlock->is_fuction_def = false;
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
        elseBlock->is_fuction_def = false;
        elseBlock->level = level;
        level++;
        if (tokens.at(index)->raw_value == "if"){
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
        if(tokens.at(index)->raw_value == "}"){
            return elseBlock;
        }
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
        printBlock->is_fuction_def = false;
        printBlock->level = level;
        size_t tempLevel = level;
        level = 0;
        if(tokens.at(index)->is_function){
            printBlock->body.push_back(buildFunction());
        } else {
            printBlock->body.push_back(buildAST());
        }
        level = tempLevel;
        return printBlock;
    } else {
        if (Data::curr_variables.count(tokens.at(index)->raw_value) || AST_Node::prev_variables.count(tokens.at(index)->raw_value)){
            cout << "Runtime error: not a function." << endl;
            exit(1);
        }
        if(tokens.at(index)->raw_value == "}" || tokens.at(index)->raw_value == ")"){
            index += 2;
            // if(tokens.at(index)->raw_value == "END"){
            return nullptr;
        }
        Expression* expressionBlock = new Expression();
        expressionBlock->is_fuction_def = false;
        expressionBlock->level = level;
        while (tokens.at(index)->raw_value != "END"){
            if (tokens.at(index)->raw_value != ";"){
                expressionBlock->body.push_back(tokens.at(index));
            }
            index++;
        }
        expressionBlock->body.push_back(tokens.at(index));
        index++;
        return expressionBlock;
    }
}

void Scrypter::calculate(){
    for(size_t i = 0; i < ASTHeads.size(); i++){
        if (!ASTHeads.at(i)->is_fuction_def){
            ASTHeads.at(i)->calculate(infixparser);
        }
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
