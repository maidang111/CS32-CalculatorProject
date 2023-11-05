#ifndef AST_H
#define AST_H
#include <iostream>
#include <string>
#include <set>
#include <variant>
#include <map>
#include <vector>

using namespace std;

class Token{
    public:
    enum type {
        BOOL,
        DOUBLE
    };
    string raw_value;
    size_t column;
    size_t row;
    double value;
    bool bool_val;
    Token* left;
    Token* right;
    Token* parent;
    bool is_operator;
    static bool error_;
    static set<string> variable_list;
    static bool outside_;
    static map<string,Token*> variable_update;
    static map<string, double> variable_value;
    static map<string, bool> variable_bool;
    type data_type;
    string get_data_type() const;


    virtual void print();
    virtual variant<bool,double> get_value();
    void delete_token(Token* node);
    void set_type(string val);
    void print_invalid_type() const;

    Token();
    virtual ~Token();
};

class Bool: public Token {
    public:
    bool is_operator = false;
    variant<bool, double> get_value();
    void print();
    Bool() {
        data_type = BOOL;
    }
};

class Add: public Token{
    public:
    bool is_operator = true;
    variant<bool,double> get_value();
    void print();
    Add() {
        data_type = DOUBLE;
    }
    
    // ~Add() = default;
};

class Subtract: public Token{
    public:
    bool is_operator = true;
    variant<bool, double> get_value();
    void print();
    Subtract() {
        data_type = DOUBLE;
    }
    // ~Subtract() = default;
};

class Mode: public Token {
    public:
    bool is_operator = true;
    variant<bool, double> get_value();
    void print();
    Mode() {
        data_type = DOUBLE;
    }
};

class Multiply: public Token{
    public:
    bool is_operator = true;
    variant<bool, double> get_value();
    void print();
    Multiply() {
        data_type = DOUBLE;
    }
    // ~Multiply() = default;
};

class Divide: public Token{
    public:
    bool is_operator = true;
    variant<bool,double> get_value();
    void print();
    Divide() {
        data_type = DOUBLE;
    }
    // ~Divide() = default;
};

class Equal: public Token{
    public:
    bool is_operator = true;
    variant<bool, double> get_value();
    void print();
    Equal() {
        data_type = DOUBLE;
    }
    // ~Equal() = default;
};

class Num: public Token{
    public:
    bool is_operator = false;
    variant<bool, double> get_value();
    void print();
    Num() {
        data_type = DOUBLE;
    }
    // ~Num() = default;

};

class Variable: public Token{
    public:
    bool is_operator = false;
    variant<bool, double> get_value();
    void print();
    Variable() {
        data_type = DOUBLE;
    }
    // ~Variable() = default;
};

class Logical: public Token {
    public:
    bool is_operator = false;
    variant<bool, double> get_value();
    void print();
    Logical() {
        data_type = BOOL;
    }
};

class Equality: public Token {
    public:
    bool is_operator = false;
    variant<bool, double> get_value();
    void print();
    Equality() {
        data_type = BOOL;
    }
};

class Comparison: public Token {
    public:
    bool is_operator = false;
    variant<bool,double> get_value();
    void print();
    Comparison() {
        data_type = BOOL;
    }
};
// class LeftP: public Token{

//     void get_value();
// };

// class RightP: public Token{
//     void get_value();
// };


#endif