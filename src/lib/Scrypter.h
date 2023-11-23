#include <iostream>
#include <iostream>
#include <vector>
#include "AST.h"
#include "Statement.h"
#include "InfixParser.h"
using namespace std;

class Scrypter{
   public:
    Scrypter(vector <Token*>);
    ~Scrypter();
    InfixParser* infixparser;
    vector <Token*> tokens;
    
    vector <Statement*> ASTHeads;
    vector <Function*> ASTFunctions;

    size_t index;
    int level;
    void delete_tokens();
    void deleteStatements();
    void delete_help(Statement* node);
    void check();
    void deleteFunc();
    void buildASTs();
    void calculate();
    private:
    Statement* buildAST();
    FunctionCall* buildFunction();
    
};