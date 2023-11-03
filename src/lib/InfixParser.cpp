#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "InfixParser.h"
#include "Node.h"
#include "Lexer.h"
#include "AST.h"
using namespace std; 

// Parsing tokens using lexer and store them in parser
InfixParser::InfixParser(){
    Lexer lexer; 
    lexer.create_endtokens();
    this->tokens = lexer.multi_end_tokens;
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
                AST->print();
                cout << endl;
                double result = AST->get_value();
                if (!Token::error_) {
                    cout << result << endl;
                    if (Token::variable_value.empty()) {
                        for (auto a: Token::variable_update) {
                            Token::variable_value.emplace(a.first, a.second->value);
                        }
                    }
                    else {
                        for (auto a: Token::variable_update) {
                            if (Token::variable_value.find(a.first) == Token::variable_value.end()) {
                                Token::variable_value.emplace(a.first, a.second->value);
                            }
                            else {
                                Token::variable_value.at(a.first) = a.second->value;
                            }
                        }
                    }
                }
                else {
                    for (auto a: Token::variable_update) {
                        if (Token::variable_value.find(a.first) != Token::variable_value.end()) {
                            a.second->value = Token::variable_value.at(a.first);
                            for (size_t j = 0; j < variables.size(); ++j) {
                                if (variables.at(j)->raw_value == a.second->raw_value) {
                                    variables.at(j)->value = a.second->value;
                                }
                            }
                        }
                        else {
                            Token::variable_list.erase(a.first);
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
        this->nextToken = tokens.at(count);
        count++;
    }
}

// Evauluates last and reverses order of operation
Token* InfixParser::parseEqual(){
    Token* equal = parseExpression();
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
            Token* equal1 = parseEqual();
            Equal* temp = new Equal;
            temp->left = equal;
            temp->right = equal1;
            if (temp->left && equal1) {
                temp->left->value = equal1->get_value();
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
            return equal;
        }
    }
}

// Evaluaties add subtraction after parsing terms for multiplication
Token* InfixParser::parseExpression(){
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
            return term;
        }
    } 
}


// Calls parseFactor and create multiplication and division nodes with the results
Token* InfixParser::parseTerm(){
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
        } else {
            return factor;
        }
    }
}

// Create tokens for variables, numbers and prioritizes parenthese when evaluating the oppertation
Token* InfixParser::parseFactor(){
    if(is_vaild == false){
        return nullptr;
    }
    if(isdigit(nextToken->raw_value[0])){
        Num* num = new Num;
        num->raw_value = nextToken->raw_value;
        scanToken();
        return num;
    } else if (isalpha(nextToken->raw_value[0]) && nextToken->raw_value != "END"){
        Variable* variable = new Variable;
        for(size_t i = 0; i < variables.size(); i++){
            if(nextToken->raw_value == variables.at(i)->raw_value){
                variable->raw_value = variables.at(i)->raw_value;
                variable->value = variables.at(i)->value;
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