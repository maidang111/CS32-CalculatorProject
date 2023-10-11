#include <iostream>
#include <string>

using namespace std; 

struct Token{
    string value = "";
    size_t column = -1;
    size_t row = -1;
};