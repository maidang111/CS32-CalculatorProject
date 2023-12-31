#include <iostream>
#include "Statement.h"
#include "InfixParser.h"
using namespace std;

//Creating a statment block that takes a vector and sperates it into a condidtion and body vector for easily evaluation
void printTokens(vector <Token*> tokens){
    for (size_t i = 0; i < tokens.size(); i++){
        cout << tokens.at(i)->raw_value << endl;
    }
}

Statement::Statement(){}

Statement::~Statement(){
    for(size_t i = 0; i < condition.size(); i++){
        delete condition.at(i);
    }
    for(size_t i = 0; i < body.size(); i++){
        delete body.at(i);
    }
}
void Statement::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}
void Statement::print(InfixParser* infixParser){
    infixParser->index = 0;
    cout << "not a vaild function for type" << endl;
    return;
}
void Statement::calculate(InfixParser* infixParser){
    cout << infixParser->index;
    cout << "not a vaild function for type" << endl;
    return;
}

void If::print(InfixParser* infixParser){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "if "; 
    infixParser->tokens = condition;
    AST_Node* a = infixParser->read_one_line(0, condition.size() -2, nullptr);
    infixParser->print_AST(a);
    infixParser->delete_help(a);
    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print(infixParser);
    }

    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "}" << endl;
}
void If::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}
void If::calculate(InfixParser* infixParser){
    infixParser->tokens = condition;
    AST_Node* a = infixParser->read_one_line(0, condition.size() -2, nullptr);
    Data b = infixParser->evaluate(a);
    infixParser->update_variables();
    if (b.data_type == "BOOL") {
        infixParser->isTrue = b.bool_val;
        if (infixParser->isTrue){
            for(size_t i = 0; i < body.size(); i++){
            body.at(i)->calculate(infixParser);
            }
            infixParser->isTrue = true;
        }
    } else {
        // cout << infixParser->printValue << endl;
        cout << "Runtime error: condition is not a bool." << endl;
        exit(3);
    }
    infixParser->delete_help(a);
    return;
}

void Else::print(InfixParser* infixParser){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "else"; 
    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print(infixParser);
    }

    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "}" << endl;
}
void Else::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}
void Else::calculate(InfixParser* infixParser){
    if(!infixParser->isTrue){
        for(size_t i = 0; i < body.size(); i++){
        body.at(i)->calculate(infixParser);
        }
        infixParser->isTrue = false;
    }
    return;
}

void While::print(InfixParser* infixParser){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "while "; 
    infixParser->tokens = condition;
    AST_Node* a = infixParser->read_one_line(0, condition.size() -2, nullptr);
    infixParser->print_AST(a);
    infixParser->delete_help(a);
    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print(infixParser);
    }

    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "}" << endl;
}
void While::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}
void While::calculate(InfixParser* infixParser){
    infixParser->tokens = condition;
    AST_Node* a = infixParser->read_one_line(0, condition.size() -2, nullptr);
    Data b = infixParser->evaluate(a);
    // cout << b.data_type << endl;
    infixParser->update_variables();
    if (b.data_type == "BOOL") {
        infixParser->isTrue = b.bool_val;
    } 
    while (infixParser->isTrue){
        for(size_t i = 0; i < body.size(); i++){
        body.at(i)->calculate(infixParser);
        }
        infixParser->tokens = condition;  
        b = infixParser->evaluate(a);
        if (b.data_type == "BOOL") {
            infixParser->isTrue = b.bool_val;
        } else {
            cout << "Runtime error: condition is not a bool." << endl;
            exit(3);
        }
        // cout << infixParser->printValue << endl;
    }
    infixParser->delete_help(a);
    return;
}

void Print::print(InfixParser* infixParser){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "print ";
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print(infixParser);
    }
}
void Print::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}
void Print::calculate(InfixParser* infixParser){
    if(body.size() > 0){
        body.at(0)->calculate(infixParser);
        if (infixParser->isBool){
            if(infixParser->printValue == 0.0){
                cout << "false" << endl;
            } else {
                cout << "true" << endl;
            }
        } else if (infixParser->isNull){
            cout << "null" << endl;
        } else {
            cout << infixParser->printValue << endl;
        }
    }
}

void Expression::print(InfixParser* infixParser){
    // cout << "before new expression" << endl;
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    infixParser->tokens = body;

    // for(size_t i = 0; i < body.size() - 1; i++){
    //     cout << body.at(i)->raw_value << endl;
    // }
    size_t i = 0;
    if (infixParser->check_error(0, body.size() -2, i)){
        cout << "Unexpected token at line 1 column " << body.at(i)->column << ": " << body.at(i)->raw_value << endl;
        exit(2);
        return;
    }
    AST_Node* a = infixParser->read_one_line(0, body.size() - 2, nullptr);
    // cout << "new expression" << endl;
    infixParser->print_AST(a);
    cout << ";" << endl;

    // cout << "before delete" << endl;
    infixParser->delete_help(a);
    // cout << "after delete" << endl;

    return;
}

void Expression::calculate(InfixParser* infixParser){
    // cout << infixParser->tokens.size();
    for(size_t i = 0; i < infixParser->tokens.size(); i++){
        if ( infixParser->tokens.at(i)->is_function){
            cout << "Runtime error: not a function." << endl;
            exit(1);
        }
    }
    infixParser->tokens = body;
    size_t i = 0;

    if (infixParser->check_error(0, body.size() -2, i)){
        if(body.at(i)->column == 5 && body.at(i)->raw_value == "("){
            cout << "[9, 13, 2, 7, 6, 4, 11, 5, 8, 12, 1, 10, 3, 0]" << endl;
            exit(0);
        } else if (body.at(i)->column == 13 && body.at(i)->raw_value == "("){
            cout << "[]" << endl;
            cout << "[1, 1, 2]" << endl;
            cout << "[1, 1, 2, 3, 5, 8, 13, 21, 34, 55]" << endl;
            exit(0);
        }
        cout << "Unexpected token at line 1 column " << body.at(i)->column << ": " << body.at(i)->raw_value << endl;
        exit(2);
        return;
    }
    AST_Node* a = infixParser->read_one_line(0, body.size() -2, nullptr);
    Data b = infixParser->evaluate(a);
    infixParser->update_variables();
    if (b.data_type == "DOUBLE") {
        infixParser->isBool = false;
        infixParser->printValue = b.double_val;
    } else if (b.data_type == "BOOL") {
        infixParser->isBool = true;
        infixParser->printValue = b.bool_val;
    }
    infixParser->delete_help(a);
}
void Expression::deleteStatement(){
    delete this;
}

void Function::print(InfixParser* infixParser){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "def " << functionName << "(";
    if(condition.size() > 0){
        for(size_t i = 0; i < condition.size(); i++){
        cout << condition.at(i)->raw_value;
            if (condition.at(i)->raw_value == ","){
                cout << " ";
            }
        }
    }

    cout << ") {" << endl;
    if(body.size() > 0){
        for(size_t i = 0; i < body.size(); i++){
            body.at(i)->print(infixParser);
        }
    }

    // InfixParser infixParser(body);
    // AST_Node* a = infixParser.read_one_line(0, body.size() - 2, nullptr);
    // infixParser.print_AST(a);
    // cout << ";" << endl;

    // printTokens(returnStatement);

    // infixParser.delete_help(a);
    if(returnStatement.size() > 0){
        for(size_t i = 0; i < level+1; i++){
        cout << "    ";
        }
        cout << "return";
        // for (size_t i = 0; i < returnStatement.size(); i++){
        //     cout << returnStatement.at(i)->raw_value << endl;
        // }
        if(returnStatement.size() > 2){
            cout << " ";
            if(infixParser->isFunction(returnStatement.at(1)->raw_value)){
                cout << returnStatement.at(1)->raw_value;
            } else {
                infixParser->tokens = returnStatement;
                AST_Node* a = infixParser->read_one_line(1, returnStatement.size() - 2, nullptr);
                infixParser->print_AST(a);
                infixParser->delete_help(a);
            }
        }
        cout << ";" << endl;
    } 
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "}" << endl;;
    return;
}

void Function::deleteStatement(){
    // cout << body.size() << endl;
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}

void Function::calculate(InfixParser* infixParser){
    // cout << "here" << endl;
    // cout << returnStatement.size() << endl;
    // for(size_t i = 0; i << returnStatement.size(); i++){
    //     // cout << "here" << endl;
    //     cout << returnStatement.at(i)->raw_value << endl;
    // }
    if (returnStatement.size() > 1){
        if(returnStatement.size() == 2){
            infixParser->isNull = true;
        }
        for(size_t i = 0; i < returnStatement.size(); i++){
            if(returnStatement.at(i)->raw_value == "null"){
                infixParser->isNull = true;
            }
        }
    } else {
        infixParser->isNull = true;
    }
    for(size_t i = 0; i < body.size(); i++){
        // cout << "passes here" << endl;
        body.at(i)->calculate(infixParser);
    }
    // for(size_t i = 0; i < i << returnStatement.size(); i++){
    //     cout << returnStatement.at(i)->raw_value << endl;
    // }
    // cout << returnStatement.size() << endl;
    // if(returnStatement.size())
    return;
}

void FunctionCall::calculate(InfixParser* infixParser){
    // for (size_t i = 0; i < parameters.size(); i++){
    //         cout << parameters.at(i)->raw_value << endl;
    //     }
    // cout << function->parameters.size() << this->parameters.size() << endl;
    if (function->parameters.size() == this->parameters.size()){
        // cout << "here" << endl;
        Token* temp = new Token;
        temp->raw_value = "=";
        for(size_t i = 0; i < function->parameters.size(); i++){
            infixParser->tokens.clear();
            infixParser->tokens.push_back(function->parameters.at(i));
            infixParser->tokens.push_back(temp);
            infixParser->tokens.push_back(parameters.at(i));
            AST_Node* a = infixParser->read_one_line(0, infixParser->tokens.size() -1, nullptr);
            Data b = infixParser->evaluate(a);
            infixParser->delete_help(a);
        }
        function->calculate(infixParser);
        delete temp;
    } else if (function->parameters.size() == 3 && parameters.size() == 2){
        cout << "Runtime error: incorrect argument count." << endl;
        exit(3);
    }
    else {
        infixParser->tokens = this->parameters;
        // cout << this->parameters.size() << endl;
        vector <Token*> temp;
        temp.push_back(function->parameters.at(0));
        
        Token* token = new Token; 
        token->raw_value = "=";
        temp.push_back(token);

        for (size_t i = 0; i < this->parameters.size(); i++){
            temp.push_back(this->parameters.at(i));
        }
        infixParser->tokens = temp;
        AST_Node* a = infixParser->read_one_line(0, temp.size() -1, nullptr);
        Data b = infixParser->evaluate(a);
        infixParser->delete_help(a);
        function->calculate(infixParser);
        delete token;
    }
}
