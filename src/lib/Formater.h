#include <iostream>
#include <vector>
#include "AST.h"
#include "Statement.h"
using namespace std;

class Formater{
    public:
    Formater(vector <Token*>);
    
    vector <Token*> tokens;
    vector <Statement*> ASTHeads;
    size_t index = 0;

    void buildASTs();
    void printFormated();
    private:
    Statement* buildAST();
    
};
