#ifndef AST_H
#define AST_H
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>

using namespace std;

class Token{
    public:
    string raw_value;
    size_t column;
    size_t row;
    double value;
    Token* left;
    Token* right;
    Token* parent;
    bool is_operator;
    static bool error_;
    static set<string> variable_list;
    static map<string,Token*> variable_update;
    static map<string, double> variable_value;

    virtual void print();
    virtual double get_value();
    void delete_token(Token* node);

    Token();
    virtual ~Token();
};

class Add: public Token{
    public:
    bool is_operator = true;
    double get_value();
    void print();
    
    // ~Add() = default;
};

class Subtract: public Token{
    public:
    bool is_operator = true;
    double get_value();
    void print();
    // ~Subtract() = default;
};

class Multiply: public Token{
    public:
    bool is_operator = true;
    double get_value();
    void print();
    // ~Multiply() = default;
};

class Divide: public Token{
    public:
    bool is_operator = true;
    double get_value();
    void print();
    // ~Divide() = default;
};

class Equal: public Token{
    public:
    bool is_operator = true;
    double get_value();
    void print();
    // ~Equal() = default;
};

class Num: public Token{
    public:
    bool is_operator = false;
    double get_value();
    void print();
    // ~Num() = default;

};

class Variable: public Token{
    public:
    bool is_operator = false;
    double get_value();
    void print();
    // ~Variable() = default;
};

// class LeftP: public Token{

//     void get_value();
// };

// class RightP: public Token{
//     void get_value();
// };


#endif
