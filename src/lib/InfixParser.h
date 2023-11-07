#include <iostream>
#include <string>
#include <vector>
#include "AST.h"

using namespace std; 

class InfixParser{
    public:
    InfixParser(vector <Token*> tokens);
    ~InfixParser();
    void delete_tokens();
    void delete_variables();
    bool error_parenthesis(size_t index);
    bool error_assignment(size_t index);
    set<string> operators;

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
    Token* parseComparison();
    Token* parseEquality();
    Token* parseLogicalAnd();
    Token* parseLogicalOrExclusive();
    Token* parseLogicalOrInclusive();
    vector <Token*> tokens;
    vector <Token*> variables;
    vector <Token*> tempVariables;
    vector <Token*> ASTheads;

    void scanToken();

    
    // build tree
    // A print function
    // calculate function
};

