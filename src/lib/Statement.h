#include <iostream>
#include <string>
#include <vector>
#include "AST.h"
#include "Lexer.h"
using namespace std;

class Statement{
    public:
    Statement();
    vector <Token*> condition;
    vector <Statement*> body;
    virtual void print();
    // virtual vector <Token*> getValue();
};

class While: public Statement{
    public:
    vector <Token*> condition;
    vector <Statement*> body;
    void print();
    // vector <Token*> getValue();
};

class If: public Statement{
    public:
    vector <Token*> condition;
    vector <Statement*> body;
    void print();
    // vector <Token*> getValue();
};

class Else: public Statement{
    public:
    vector <Token*> condition;
    vector <Statement*> body;
    void print();
    // vector <Token*> getValue();
};

class Print: public Statement{
    public:
    vector <Statement*> body;
    void print();
    // vector <Token*> getValue();
};

class Expression: public Statement{
    public:
    vector <Token*> body;
    void print();
    // vector <Token*> getValue();
};