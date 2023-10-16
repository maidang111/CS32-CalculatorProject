#ifndef OPERATOR_H
#define OPERATOR_H
#include "AST.h"
#include "Node.h"
#include <vector>
#include <string>

using namespace std;

class Operator: public Node {
    private:
    string operator_value;

    public: 
    Operator(Node* parent_node, Token* in_token);
    bool node_type() const;
    string check_operator() const;
    ~Operator();
    double get_number() const;

};

#endif