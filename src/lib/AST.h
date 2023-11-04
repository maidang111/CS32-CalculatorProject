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
    enum type {
        BOOL,
        DOUBLE, 
        END
    };
    string raw_value;
    size_t column;
    size_t row;
    double value;
    Token* left;
    Token* right;
    Token* parent;
    bool is_operator;
    bool bool_val;
    static bool error_;
    static set<string> variable_list;
    static bool outside_;
    static map<string,Token*> variable_update;
    static map<string, double> variable_value;
    type data_type;


    virtual void print();
    virtual double get_value();
    virtual bool get_value_bool();
    void set_token_type(string a);
    void delete_token(Token* node);
    void set_type(string val);
    void set_type(Token* assign);
    string return_type() const;

    Token();
    virtual ~Token();
};

class Logical: public Token {
    public:
    bool is_operator = false;
    bool bool_val = true;
    bool get_value_bool(); // template class/union
    void print();
};

class Equality: public Token {
    public:
    bool is_operator = false;
    bool bool_val = true;
    bool get_value_bool(); //template class/union
    void print();
};

class Comparison: public Token{
    public:
    bool is_operator = false;
    bool bool_val = true;
    bool get_value_bool(); // template class/ union
    void print(); 
};

class Add: public Token{
    public:
    bool is_operator = true;
    bool bool_val = false;
    double get_value();
    void print();
    
    // ~Add() = default;
};

class Subtract: public Token{
    public:
    bool is_operator = true;
    bool bool_val = false;
    double get_value();
    void print();
    // ~Subtract() = default;
};

class Multiply: public Token{
    public:
    bool is_operator = true;
    bool bool_val = false;
    double get_value();
    void print();
    // ~Multiply() = default;
};

class Divide: public Token{
    public:
    bool is_operator = true;
    bool bool_val = false;
    double get_value();
    void print();
    // ~Divide() = default;
};

class Mode: public Token {
    public:
    bool is_operator = true;
    double bool_val = false;
    double get_value();
    void print();
};

template <typename T> 
class Equal: public Token{
    public:
    bool is_operator = true;
    bool bool_val = false;
    T get_value();
    void print();
    // ~Equal() = default;
};

class Num: public Token{
    public:
    bool is_operator = false;
    bool bool_val = false;
    double get_value();
    void print();
    // ~Num() = default;

};

class Bool: public Token {
    public:
    bool is_operator = false;
    bool bool_val = true;
    bool get_value_bool(); //template class needed here
    void print();
};

class Variable: public Token{
    public:
    bool is_operator = false;
    bool bool_val = false;
    void set_bool_val(string a) {
        if (a == "true" || a == "false") {
            bool_val = true;
            return;
        }
        bool_val = false;
    }
    double get_value();
    bool get_value_bool(); 
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
