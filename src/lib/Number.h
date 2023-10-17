#ifndef NUMBER_H
#define NUMBER_H
#include "AST.h"
#include "Node.h"
#include <string>
using namespace std;

class Number: public Node {
    double number_value;

    public: 
    Number(Node* parent_token, Token* in_token);
    bool node_type() const;
    double get_number() const; 
    string check_operator() const {return "";};
    ~Number();
};

#endif