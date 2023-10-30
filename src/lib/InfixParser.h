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

    void build_AST();
    private:
    Token* nextToken;
    size_t count;
    Token* AST;
    Token* parseExpression();
    Token* parseFactor();
    Token* parseTerm();
    vector <Token*> tokens;

    vector <Token> variables;

    void scanToken();

    
    // build tree
    // A print function
    // calculate function
};

