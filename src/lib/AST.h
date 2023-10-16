#ifndef AST_H
#define AST_H
#include <iostream>
#include <string>
#include <set>

struct Token{
    std::string value;
    size_t column;
    size_t row;

    Token();
};

#endif
