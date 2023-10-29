#include <iostream>
#include <string>
#include <vector>
#include "AST.h"

using namespace std; 

class InfixParser{
    public:
    InfixParser();
    ~InfixParser();
    
    void build_AST();
    private:
    Token* nextToken;
    size_t count;
    Token* AST;
    Token* parseExpression();
    Token* parseFactor();
    Token* parseTerm();
    vector <Token*> tokens;

    void scanToken();
    
    // build tree
    // A print function
    // calculate function
};

