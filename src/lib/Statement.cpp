#include <iostream>
#include "Statement.h"
#include "InfixParser.h"
using namespace std;

//Creating a statment block that takes a vector and sperates it into a condidtion and body vector for easily evaluation
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
void Statement::print(){
    cout << "not a vaild function for type" << endl;
    return;
}
void Statement::calculate(InfixParser* infixParser){
    cout << infixParser->index;
    cout << "not a vaild function for type" << endl;
    return;
}

void If::print(){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "if "; 
    InfixParser infixParser(condition);
    AST_Node* a = infixParser.read_one_line(0, condition.size() -2, nullptr);
    infixParser.print_AST(a);
    infixParser.delete_help(a);

    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print();
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
    cout << infixParser->index;
    return;
}

void Else::print(){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "else"; 
    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print();
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
    cout << infixParser->index;
    return;
}

void While::print(){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "while "; 
    InfixParser infixParser(condition);
    AST_Node* a = infixParser.read_one_line(0, condition.size() -2, nullptr);
    infixParser.print_AST(a);
    infixParser.delete_help(a);
    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print();
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
    // for(size_t i = 0; i < condition.size(); i++){
    //     cout << condition.at(i)->raw_value << endl;
    // }
    AST_Node* a = infixParser->read_one_line(0, condition.size() -2, nullptr);
    Data b = infixParser->evaluate(a);
    // cout << b.data_type << endl;
    infixParser->update_variables();
    if (b.data_type == "BOOL") {
        infixParser->isTrue = b.bool_val;
    }
    while (infixParser->isTrue){
        // cout << "here" << endl;
        for(size_t i = 0; i < body.size(); i++){
        body.at(i)->calculate(infixParser);
        b = infixParser->evaluate(a);
        infixParser->update_variables();
        infixParser->isTrue = b.bool_val;
        }
    }
    return;
}

void Print::print(){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    cout << "print ";
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print();
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
        cout << infixParser->printValue << endl;
    }
}

void Expression::print(){
    for(size_t i = 0; i < level; i++){
        cout << "    ";
    }
    InfixParser infixParser(body);
    AST_Node* a = infixParser.read_one_line(0, body.size() -2, nullptr);
    infixParser.print_AST(a);
    infixParser.delete_help(a);
    cout << endl;
}

void Expression::calculate(InfixParser* infixParser){
    // cout << infixParser->tokens.size();
    infixParser->tokens = body;
    AST_Node* a = infixParser->read_one_line(0, body.size() -2, nullptr);
    Data b = infixParser->evaluate(a);
    infixParser->update_variables();
    if (b.data_type == "DOUBLE") {
        infixParser->printValue = b.double_val;
    }
    infixParser->delete_help(a);
}
void Expression::deleteStatement(){
    delete this;
}
