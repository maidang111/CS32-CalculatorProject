#include <iostream>
#include "lib/Lexer.h"
#include "lib/Formater.h"
#include <string>

using namespace std; 

int main(){
    string line = "";
    while (!cin.eof()){
        getline(cin, line);
        cout << line << endl;
        // for testing
        // cout << line << endl;
    }
    return 0;
}