#ifndef AST_H
#define AST_H
#include <iostream>
#include <string>
#include <set>

using namespace std;

class Token{
    public:
    string raw_value;
    size_t column;
    size_t row;
    double value;
    Token* left;
    Token* right;

    virtual void print();
    virtual double get_value();
    void deleteToken();

    Token();
    virtual ~Token() = default;
};

class Add: public Token{
    public:
    double get_value();
    void print();
    
    ~Add() = default;
};

class Subtract: public Token{
    public:
    double get_value();
    void print();
    ~Subtract() = default;
};

class Multiply: public Token{
    public:
    double get_value();
    void print();
    ~Multiply() = default;
};

class Divide: public Token{
    public:
    double get_value();
    void print();
    ~Divide() = default;
};

class Equal: public Token{
    public:
    double get_value();
    void print();
    ~Equal() = default;
};

class Num: public Token{
    public:
    double get_value();
    void print();
    ~Num() = default;

};

class Variable: public Token{
    public:
    double get_value();
    void print();
    void deleteToken();
    ~Variable() = default;
};

// class LeftP: public Token{

//     void get_value();
// };

// class RightP: public Token{
//     void get_value();
// };


#endif
