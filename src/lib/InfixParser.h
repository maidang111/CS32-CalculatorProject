#include <iostream>
#include <string>
#include <vector>
#include "AST.h"

using namespace std; 

class InfixParser{
    public:
    InfixParser();
    ~InfixParser();
    void delete_tokens();
    void delete_variables();

    void build_AST();
    private:
    Token* nextToken;
    size_t count;
    Token* AST;
    Token* parseExpression();
    Token* parseFactor();
    Token* parseTerm();
    Token* parseEqual();
    vector <Token*> tokens;
    vector <Token*> variables;
    vector <Token*> ASTheads;

    void scanToken();
    void prevToken();

    
    // build tree
    // A print function
    // calculate function
};

