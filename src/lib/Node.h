#ifndef NODE_H
#define NODE_H
#include "AST.h"
#include <vector>

using namespace std;

class Node {
    protected:
    Node* parent;
    Token* data;

    public:
    Node();
    virtual ~Node();
    virtual bool node_type() const;
};

#endif
