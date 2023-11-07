#include <iostream>
#include "Statement.h"
#include "InfixParser.h"
using namespace std;

//Creating a statment block that takes a vector and sperates it into a condidtion and body vector for easily evaluation
Statement::Statement(){}
void Statement::print(){
    cout << "not a vaild function for type" << endl;
    return;
}
void If::print(){
    cout << "if "; 
    condition.at(0)->print();
    cout << "{" << endl;
    for(size_t i = 0; i < body.size(); i++){
        cout << "    ";
        body.at(i)->print();
        cout << endl;
    }
    cout << "}" << endl;
}
void Else::print(){
    cout << "else "; 
    condition.at(0)->print();
    cout << "{" << endl;
    for(size_t i = 0; i < body.size(); i++){
        cout << "    ";
        body.at(i)->print();
        cout << endl;
    }
    cout << "}" << endl;
}
void While::print(){
    cout << "while "; 
    // cout << condition.size();
    condition.at(0)->print();
    cout << "{" << endl;
    for(size_t i = 0; i < body.size(); i++){
        cout << "    ";
        body.at(i)->print();
    }
    cout << "}" << endl;
}
void Print::print(){
    cout << "print ";
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print();
    }
}
void Expression::print(){
    InfixParser infixParser(body);
    infixParser.build_AST();
}