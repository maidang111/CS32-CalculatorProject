#include <iostream>
#include <string>
#include <set>

using namespace std; 

struct Lexer{
    string text;
    set <char> possible_values;

    Lexer(string text);

    void create_tokens();

    void print_tokens();
};