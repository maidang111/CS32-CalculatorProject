#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <variant>
#include "InfixParser.h"
#include "Node.h"
#include "Lexer.h"
#include "AST.h"
using namespace std; 

// Parsing tokens using lexer and store them in parser
InfixParser::InfixParser(vector <Token*> tokens){
    for(size_t i = 0; i << tokens.size(); i ++){
        this->tokens.push_back(tokens.at(i));
    }
    this->tokens = tokens;
    this->count = 0;
    operators = {"+", "-", "*", "/"};
}

InfixParser::~InfixParser(){}

// Checks for number of parenthesis and checks if it's a vaild input
bool InfixParser::error_parenthesis(size_t index) {
    bool error_parenthesis = false;
    int num_parenthesis = 0;
    bool print_error = false;
    if (index != 0) {
        index -= 1;
    }

    for (size_t i = index; i < tokens.size(); ++i) {
        if (tokens.at(i)->raw_value == "(") {
            num_parenthesis += 1;
        }
        else if (tokens.at(i)->raw_value == ")") {
            num_parenthesis -= 1;
        }
        if (num_parenthesis < 0 && !print_error) {
            error_parenthesis = true;
            print_error = true;
            cout << "Unexpected token at line 1 column " << tokens.at(i)->column << ": " << tokens.at(i)->raw_value << endl;
        }
        if (tokens.at(i)->raw_value == "END") {
            if (num_parenthesis > 0 && !print_error) {
                print_error = true;
                cout << "Unexpected token at line 1 column " << tokens.at(i)->column << ": " << tokens.at(i)->raw_value << endl;
                error_parenthesis = true;
            }
            if (error_parenthesis) {
                count = i;
            }
            break;
        }
    }
    return error_parenthesis;
}

bool InfixParser::error_assignment(size_t index) {
    bool is_error = false;
    bool not_variable = false;
    int num_parenthesis = 0;
    Token* last_error = nullptr;
    vector<int> assign_parenthesis;
    if (index != 0) {
        index -= 1;
    }

    for (size_t i = index; i < tokens.size(); ++i) {
        if (tokens.at(i)->raw_value == "=") {
            if (i > 0) {
                if (!isalpha(tokens.at(i - 1)->raw_value.at(0)) && tokens.at(i - 1)->raw_value.at(0) != '_') {
                    is_error = true;
                    last_error = tokens.at(i);
                }
            }
            assign_parenthesis.push_back(num_parenthesis);
        }
        // (a =) case, no right value
        if (tokens.at(i)->raw_value == ")") {
            --num_parenthesis;
            if (!assign_parenthesis.empty()) {
                if (num_parenthesis < assign_parenthesis.at(assign_parenthesis.size() - 1)) {
                    assign_parenthesis.pop_back();
                    //
                }
            }
            if (i > 0) {
                if (tokens.at(i - 1)->raw_value == "=" || tokens.at(i - 1)->raw_value == "(") {
                    is_error = true;
                    // cout << 1 << endl;
                    last_error = tokens.at(i);
                }
            }
        }
        // number case
        if (not_variable && tokens.at(i)->raw_value == "=") {
            is_error = true;
            // cout << 2 << endl;
            last_error = tokens.at(i);
        }
        // (= a) case, no left value
        if (tokens.at(i)->raw_value == "(") {
            num_parenthesis++;
            not_variable = false;
            if (i + 1 < tokens.size()) {
                if (tokens.at(i)->raw_value == "=") {
                    is_error = true;
                    // cout << 3 << endl;
                    last_error = tokens.at(i);
                }
            }
        }
        // (1 = a) case, left side number
        if (isdigit(tokens.at(i)->raw_value.at(0)) || operators.count(tokens.at(i)->raw_value)) {
            if (assign_parenthesis.empty()) {
                not_variable = true;
            }
            else if (num_parenthesis <= assign_parenthesis.at(assign_parenthesis.size() - 1)) {
                not_variable = true;
            }

        }
        if (tokens.at(i)->raw_value == "END") {
            if (last_error) {
                count = i;
                cout << "Unexpected token at line 1 column " << last_error->column << ": " << last_error->raw_value << endl;
                return true;
            }
            break;
        }
    }
    return is_error;
}


void InfixParser::build_AST(){
    // checking problem in lexer
    // cout << "start checking lexer: " << endl;
    // for (size_t a = 0; a < tokens.size(); ++a) {
    //     cout << tokens.at(a)->raw_value << endl;
    // }
    // cout << "finished checking" << endl;
    while(count != tokens.size()){
        scanToken();
        is_vaild = true;
        bool check_parenthesis = error_parenthesis(count);
        if (check_parenthesis) {
            Token::outside_ = true;
            continue;
        }
        else {
            Token::outside_ = false;
            bool check_assignment = error_assignment(count);
            if (check_assignment) {
                Token::outside_ = true;
                continue;
            }
        }
        if (nextToken->raw_value != "END"){
            AST = parseEqual();
            if (nextToken->raw_value != "END" || is_vaild == false){
                cout << "Unexpected token at line 1" << " column " << nextToken->column << ": " << nextToken->raw_value << endl;
                while(nextToken->raw_value != "END"){
                    scanToken();
                }
            } else {
                // cout << "check AST" << endl;
                AST->print();
                cout << endl;
                // cout << "finish checking" << endl;
                bool a;
                variant<bool, double> result = AST->get_value();
                if (!Token::error_) {
                    if (holds_alternative<bool>(result)) {
                        a = get<bool>(result);
                        if (a) {
                            cout << "true" << endl;
                        }
                        else {
                            cout << "false" << endl;
                        }
                    }
                    else if (holds_alternative<double>(result)) {
                        cout << get<double>(result) << endl;
                    }
                    // cout << result << endl;
                    if (!Token::variable_bool.empty() || !Token::variable_value.empty()) {
                        auto k = Token::variable_value.begin();
                        auto l = Token::variable_bool.begin();
                        for (auto a: Token::variable_update) {
                            k = Token::variable_value.find(a.first);
                            l = Token::variable_bool.find(a.first);
                            if ((k == Token::variable_value.end()) && (l == Token::variable_bool.end())) {
                                Token::variable_update.erase(a.first);
                            }
                            else if (k != Token::variable_value.end()){
                                a.second->set_type("double");
                                a.second->value = k->second;
                            }
                            else {
                                a.second->set_type("true");
                                a.second->bool_val = l->second;
                            }
                        }
                    }
                }
                else {
                    auto k = Token::variable_value.begin();
                    auto l = Token::variable_bool.begin();
                    for (auto a: Token::variable_update) {
                        k = Token::variable_value.find(a.first);
                        l = Token::variable_bool.find(a.first);
                        if (k == Token::variable_value.end() && l == Token::variable_bool.end()) {
                            if (a.second->get_data_type() == "BOOL") {
                                Token::variable_bool.emplace(a.first, a.second->bool_val);
                            }
                            else  {
                                Token::variable_bool.emplace(a.first, a.second->value);
                            }
                        }
                        else {
                            if (a.second->get_data_type() == "BOOL") {
                                if (l != Token::variable_bool.end()) {
                                    l->second = a.second->bool_val;
                                }
                                else {
                                    k->second = a.second->value;
                                    a.second->set_type("true");
                                    Token::variable_bool.emplace(k->first, k->second);
                                    Token::variable_value.erase(k->first);
                                }
                            }
                            else {
                                if (l != Token::variable_bool.end()) {
                                    l->second = a.second->bool_val;
                                    a.second->set_type("double");
                                    Token::variable_value.emplace(l->first, l->second);
                                    Token::variable_bool.erase(l->first);
                                }
                                else {
                                    k->second = a.second->bool_val;
                                }
                            }

                        }
                    }
                }
                Token::error_ = false;
                ASTheads.push_back(AST);
            }
        } 
    }
    for(size_t i = 0; i < ASTheads.size(); i++){
        ASTheads.at(i)->delete_token(ASTheads.at(i));
    }
}

void InfixParser::scanToken(){
    if (count != tokens.size()){
        // cout << "ScanToken(): Token moved from " << tokens.at(count)->raw_value;
        this->nextToken = tokens.at(count);
        count++;
        // cout << " to " << tokens.at(count)->raw_value;
    }
}

// Evauluates last and reverses order of operation
Token* InfixParser::parseEqual(){
    // cout << "parseEqual()" << endl;
    Token* equal = parseLogicalOrInclusive();
    if(is_vaild == false){
        equal->delete_token(equal);
        return nullptr;
    }
    while (true){
        if (nextToken == nullptr){
            cout << "null expression" << endl;
            is_vaild = false;
            exit(1);
        } else if(nextToken->raw_value == "="){
            scanToken();
            Token* equal1 = parseLogicalOrInclusive();
            Equal* temp = new Equal;
            temp->left = equal;
            temp->right = equal1;
            if (temp->left && equal1) {
                if (holds_alternative<double>(equal1->get_value())) {
                    temp->left->value = get<double>(equal1->get_value());
                    temp->left->set_type("true");
                }
                else if (holds_alternative<bool>(equal1->get_value())) {
                    temp->left->bool_val = get<bool>(equal1->get_value());
                    temp->left->set_type("double");
                }
                // temp->left->value = equal1->get_value();
                for(size_t i = 0; i < variables.size(); i++){
                    if(variables.at(i)->raw_value == temp->left->raw_value){
                        variables.erase(variables.begin()+i);
                    }
                }
                variables.push_back(temp->left);      
                equal = temp;
            }
            else {
                Token::outside_ = true;
                Token a;
                is_vaild = false;
                a.delete_token(equal1);
                a.delete_token(temp);
                return nullptr;
            }
        } else {
            // cout << "equal==:  " << equal->raw_value << endl;
            return equal;
        }
    }
}
// completed parse functions for bool 
// need to implement evaluation for bool
Token* InfixParser::parseLogicalOrInclusive() {
    // cout << "parseLogicalOrInclusive" << endl;
    Token* bool_expression1 = parseLogicalOrExclusive();
    if (is_vaild == false) {
        bool_expression1->delete_token(bool_expression1);
    }
    while (true) {
        if (nextToken == nullptr) {
            cout << "null expression" << endl;
            is_vaild = false;
            exit(1);
        }
        else if (nextToken->raw_value == "|") {
            scanToken();
            Token* bool_expression2 = parseLogicalOrExclusive();
            if (!bool_expression1 || !bool_expression2) {
                Token a;
                a.delete_token(bool_expression1);
                a.delete_token(bool_expression2);
                return nullptr;
            }
            Logical* temp = new Logical();
            temp->left = bool_expression1;
            temp->right = bool_expression2;
            bool_expression1 = temp;
            bool_expression1->raw_value = "|";
        }
        else {
            return bool_expression1;
            // cout << "bool | :  " << bool_expression1->raw_value << endl;
        }
    }  
}

Token* InfixParser::parseLogicalOrExclusive() {
    // cout << "parseLogicalOrExclusive" << endl;
    Token* bool_expression1 = parseLogicalAnd();
    if (is_vaild == false) {
        bool_expression1->delete_token(bool_expression1);
    }
    while (true) {
        if (nextToken == nullptr) {
            cout << "null expression" << endl;
            is_vaild = false;
            exit(1);
        }
        else if (nextToken->raw_value == "^") {
            scanToken();
            Token* bool_expression2 = parseLogicalAnd();
            if (!bool_expression1 || !bool_expression2) {
                Token a;
                a.delete_token(bool_expression1);
                a.delete_token(bool_expression2);
                return nullptr;
            }
            Logical* temp = new Logical();
            temp->left = bool_expression1;
            temp->right = bool_expression2;
            bool_expression1 = temp;
            bool_expression1->raw_value = "^";
        }
        else {
            // cout << "bool ^ :  " << bool_expression1->raw_value << endl;
            return bool_expression1;
        }
    }  
}

Token* InfixParser::parseLogicalAnd() {
    // cout << "parseLogicalAnd" << endl;
    Token* bool_expression1 = parseEquality();
    if (is_vaild == false) {
        bool_expression1->delete_token(bool_expression1);
    }
    while (true) {
        if (nextToken == nullptr) {
            cout << "null expression" << endl;
            is_vaild = false;
            exit(1);
        }
        else if (nextToken->raw_value == "&") {
            scanToken();
            Token* bool_expression2 = parseEquality();
            if (!bool_expression1 || !bool_expression2) {
                Token a;
                a.delete_token(bool_expression1);
                a.delete_token(bool_expression2);
                return nullptr;
            }
            Logical* temp = new Logical();
            temp->left = bool_expression1;
            temp->right = bool_expression2;
            bool_expression1 = temp;
            bool_expression1->raw_value = "&";
        }
        else {
            // cout << "bool & :  " << bool_expression1->raw_value << endl;
            return bool_expression1;
        }
    }  
}

Token* InfixParser::parseEquality() {
    // cout << "parseEquality" << endl;
    Token* equality1 = parseComparison();
    if (is_vaild == false) {
        equality1->delete_token(equality1);
    }
    while (true) {
        if (nextToken == nullptr) {
            cout << "null expression" << endl;
            is_vaild = false;
            exit(1);
        }
        else if (nextToken->raw_value == "==") {
            scanToken();
            Token* equality2 = parseComparison();
            if (!equality1 || !equality2) {
                Token a;
                a.delete_token(equality1);
                a.delete_token(equality2);
                return nullptr;
            }
            Equality* temp = new Equality();
            temp->left = equality1;
            temp->right = equality2;
            equality1 = temp;
            equality1->raw_value = "==";
        }
        else if (nextToken->raw_value == "!=") {
            scanToken();
            Token* equality2 = parseComparison();
            if (!equality1 || !equality2) {
                Token a;
                a.delete_token(equality1);
                a.delete_token(equality2);
                return nullptr;
            }
            Equality* temp = new Equality();
            temp->left = equality1;
            temp->right = equality2;
            equality1 = temp;
            equality1->raw_value = "!=";
        }
        else {
            // cout << "equality1 :  " << equality1->raw_value << endl;
            return equality1;
        }
    }  
}

Token* InfixParser::parseComparison() {
    // cout << "parseComparison" << endl;
    Token* comparison1 = parseExpression();
    if (is_vaild == false) {
        comparison1->delete_token(comparison1);
    }
    while (true) {
        if (nextToken == nullptr) {
            cout << "null expression" << endl;
            is_vaild = false;
            exit(1);
        }
        else if (nextToken->raw_value == "<") {
            // cout << nextToken->raw_value << " ";
            scanToken();
            Token* comparison2 = parseExpression();
            if (!comparison1 || !comparison2) {
                Token a;
                a.delete_token(comparison1);
                a.delete_token(comparison2);
                return nullptr;
            }
            Comparison* temp = new Comparison();
            temp->left = comparison1;
            // cout << "temp->left: " << comparison1->raw_value << endl;
            temp->right = comparison2;
            // cout << "temp->right: " << comparison2->raw_value << endl;
            comparison1 = temp;
            comparison1->raw_value = "<";
            // cout << "check c1: " << comparison1->raw_value << endl;
            // cout << "added" << endl;
        }
        else if (nextToken->raw_value == "<=") {
            scanToken();
            Token* comparison2 = parseExpression();
            if (!comparison1 || !comparison2) {
                Token a;
                a.delete_token(comparison1);
                a.delete_token(comparison2);
                return nullptr;
            }
            Comparison* temp = new Comparison();
            temp->left = comparison1;
            temp->right = comparison2;
            comparison1 = temp;
            comparison1->raw_value = "<=";
        }
        else if (nextToken->raw_value == ">") {
            scanToken();
            Token* comparison2 = parseExpression();
            if (!comparison1 || !comparison2) {
                Token a;
                a.delete_token(comparison1);
                a.delete_token(comparison2);
                return nullptr;
            }
            Comparison* temp = new Comparison();
            temp->left = comparison1;
            temp->right = comparison2;
            comparison1 = temp;
            comparison1->raw_value = ">";
        }
        else if (nextToken->raw_value == ">=") {
            scanToken();
            Token* comparison2 = parseExpression();
            if (!comparison1 || !comparison2) {
                Token a;
                a.delete_token(comparison1);
                a.delete_token(comparison2);
                return nullptr;
            }
            Comparison* temp = new Comparison();
            temp->left = comparison1;
            temp->right = comparison2;
            comparison1 = temp;     
            comparison1->raw_value = ">=";       
        }
        else {
            // cout << "bool comparison :  " << comparison1->raw_value << endl;
            return comparison1;
        }
    }
}

// Evaluaties add subtraction after parsing terms for multiplication
Token* InfixParser::parseExpression(){
    // cout << "parseExpression" << endl;
    Token* term = parseTerm();
    if(is_vaild == false){
        term->delete_token(term);
        return nullptr;
    }
    while (true){
        if (nextToken == nullptr){
            cout << "null expression" << endl;
            is_vaild = false;
            exit(1);
        } else if(nextToken->raw_value == "+"){
            scanToken();
            Token* term1 = parseTerm();
            if (!term || !term1) {
                Token a;
                a.delete_token(term);
                a.delete_token(term1);
                return nullptr;
            }
            Add* temp = new Add;
            temp->left = term;
            temp->right = term1;
            term = temp;
        } else if(nextToken->raw_value == "-"){
            scanToken();
            Token* term1 = parseTerm();
            if (!term || !term1) {
                Token a;
                a.delete_token(term);
                a.delete_token(term1);
                return nullptr;
            }
            Subtract* temp = new Subtract;
            temp->left = term;
            temp->right = term1;
            term = temp;
        } else {
            // cout << "double + - :  " << term->raw_value << endl;
            return term;
        }
    } 
}


// Calls parseFactor and create multiplication and division nodes with the results
Token* InfixParser::parseTerm(){
    // cout << "parseTerm" << endl;
    Token* factor = parseFactor();
    if(is_vaild == false){
        factor->delete_token(factor);
        return nullptr;
    }
    while (true){
        if (nextToken == nullptr){
            cout << "null expression" << endl;
            is_vaild = false;
            exit(1);
        } else if(nextToken->raw_value == "*"){
            scanToken();
            Token* factor1 = parseFactor();
            if (!factor || !factor1) {
                Token a;
                a.delete_token(factor);
                a.delete_token(factor1);
                return nullptr;
                return nullptr;
            }
            Multiply* temp = new Multiply;
            temp->left = factor;
            temp->right = factor1;
            factor = temp;
        } else if(nextToken->raw_value == "/"){
            scanToken();
            Token* factor1 = parseFactor();
            if (!factor || !factor1) {
                Token a;
                a.delete_token(factor);
                a.delete_token(factor1);
                return nullptr;
            }
            Divide* temp = new Divide;
            temp->left = factor;
            temp->right = factor1;
            factor = temp;
        } else if (nextToken->raw_value == "%") {
            scanToken();
            Token* factor1 = parseFactor();
            if (!factor || !factor1) {
                Token a;
                a.delete_token(factor);
                a.delete_token(factor1);
                return nullptr;
            }
            Mode* temp = new Mode;
            temp->left = factor;
            temp->right = factor1;
            factor = temp;
        }
        else {
            // cout << "double * / :  " << factor->raw_value << endl;
            return factor;
        }
    }
}

// Create tokens for variables, numbers and prioritizes parenthese when evaluating the oppertation
Token* InfixParser::parseFactor(){
    // cout << "parseFactor" << endl;
    if(is_vaild == false){
        return nullptr;
    }
    if(isdigit(nextToken->raw_value[0])){
        //
        // cout << "number: " << nextToken->raw_value << " ";
        Num* num = new Num;
        num->raw_value = nextToken->raw_value;
        num->value = stod(nextToken->raw_value);
        scanToken();
        return num;
    }
    else if (nextToken->raw_value == "print" || nextToken->raw_value == "if") {
        Token::error_ = true;
        is_vaild = false;
        return nullptr;
    }
    else if (nextToken->raw_value == "true" || nextToken->raw_value == "false") {
        Bool* val = new Bool();
        val->raw_value = nextToken->raw_value;
        if (nextToken->raw_value == "true") {
            val->bool_val = true;
        }
        else {
            val->bool_val = false;
        }
        scanToken();
        return val;
    } else if (isalpha(nextToken->raw_value[0]) && nextToken->raw_value != "END"){
        Variable* variable = new Variable;
        for(size_t i = 0; i < variables.size(); i++){
            if(nextToken->raw_value == variables.at(i)->raw_value){
                variable->raw_value = variables.at(i)->raw_value;
                //
                if (variables.at(i)->get_data_type() == "BOOL") {
                    variable->bool_val = variables.at(i)->bool_val;
                    variable->set_type("true");
                }
                else if (variables.at(i)->get_data_type() == "DOUBLE") {
                    variable->value = variables.at(i)->value;
                    variable->set_type("double");   
                }
                scanToken();
                return variable;
            }
        }
        variable->raw_value = nextToken->raw_value;
        scanToken();
        return variable;
    } else if (nextToken->raw_value == "("){
        scanToken();
        if(!isdigit(nextToken->raw_value[0]) && !isalpha(nextToken->raw_value[0]) && (nextToken->raw_value != "(")){
            return nullptr;
        }
        Token* expression = parseEqual();
        if (expression == nullptr) {
            is_vaild = false;
            return nullptr;
        }
        if(nextToken->raw_value == ")"){
            scanToken();
            return expression;
        } else {
            
            is_vaild = false;
            return nullptr;
        }
    } else {
        // cout << "n" << endl;
        is_vaild = false;
        return nullptr;
    }
}

void InfixParser::delete_tokens(){
    for(size_t i = 0; i < tokens.size(); i++){
        delete tokens.at(i);
    }
}

void InfixParser::delete_variables(){
    for(size_t i = 0; i < variables.size(); i++){
        delete variables.at(i);
    }
}