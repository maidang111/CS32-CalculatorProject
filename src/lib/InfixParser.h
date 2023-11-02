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
    bool error_parenthesis(size_t index);

    void build_AST();
    private:
    bool is_vaild = true;
    // bool leftparethese = false;
    Token* nextToken;
    size_t count;
    Token* AST;
    Token* parseExpression();
    Token* parseFactor();
    Token* parseTerm();
    Token* parseEqual();
    vector <Token*> tokens;
    vector <Token*> variables;
    vector <Token*> tempVariables;
    vector <Token*> ASTheads;

    void scanToken();

    
    // build tree
    // A print function
    // calculate function
};

