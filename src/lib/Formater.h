#include <iostream>
#include <vector>
#include "AST.h"
#include "Statement.h"
using namespace std;

class Formater{
    public:
    Formater(vector <Token*>);
    ~Formater();
    
    vector <Token*> tokens;
    vector <Statement*> ASTHeads;
    size_t index;
    int level;
    void delete_tokens();
    void deleteStatements();
    void delete_help(Statement* node);
    void check();
    void deleteFunc();
    void buildASTs();
    void printFormated();
    private:
    Statement* buildAST();
};
