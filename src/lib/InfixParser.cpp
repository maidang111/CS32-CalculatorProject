#include <iostream>
#include <string>
#include <vector>
#include "InfixParser.h"
#include "Node.h"
#include "Lexer.h"
#include "AST.h"


using namespace std; 


InfixParser::InfixParser(){

    Lexer lexer; 

    lexer.create_endtokens();

    this->tokens = lexer.multi_end_tokens;
    // cout << "checking lexer: " << endl;
    // for (auto a: lexer.multi_end_tokens) {
    //     cout << "row: " << a->row << endl;
    // }
    // cout << "finish checking" << endl;

    this->count = 0;

}


InfixParser::~InfixParser(){}

bool InfixParser::error_parenthesis(size_t index) {
    bool error_parenthesis = false;
    int num_parenthesis = 0;
    if (index != 0) {
        index -= 1;
    }
    for (size_t i = index; i < tokens.size(); ++i) {
        // cout << num_parenthesis << endl;
        cout << tokens.at(i)->row << endl;
        if (tokens.at(i)->raw_value == "(") {
            // cout << 3 << endl;
            num_parenthesis += 1;
        }
        else if (tokens.at(i)->raw_value == ")") {
            num_parenthesis -= 1;
        }
        if (num_parenthesis < 0) {
            error_parenthesis = true;
            // cout<< 2 << endl;
            cout << "Unexpected token at line " << tokens.at(i)->row << " column " << tokens.at(i)->column << ": " << tokens.at(i)->raw_value << endl;
        }
        if (tokens.at(i)->raw_value == "END") {
            if (num_parenthesis > 0) {
                // cout << 1 << endl;
                cout << "Unexpected token at line " << tokens.at(i)->row << " column " << tokens.at(i)->column << ": " << tokens.at(i)->raw_value << endl;
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

void InfixParser::build_AST(){

    while(count != tokens.size()){
        // cout << "Token size: " << tokens.size() << endl;

        scanToken();

        is_vaild = true;
        // cout << "before check " << count << endl;
        bool check_parenthesis = error_parenthesis(count);
        // cout << "after check: " << count << endl;
        if (check_parenthesis) {
            continue;
        }
        if (nextToken->raw_value != "END"){

            AST = parseEqual();

            if (nextToken->raw_value != "END" || is_vaild == false){
                // cout << 10 << endl;
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

//    AST->get_value();

}


void InfixParser::scanToken(){

    if (count != tokens.size()){

        this->nextToken = tokens.at(count);

        count++;

    }

}


Token* InfixParser::parseEqual(){

    // if (nextToken->raw_value == ")"){

    //     cout << "Unexpected token at line 1" << " column " << nextToken->column << ": " << nextToken->raw_value << endl;

    //     is_vaild = false;

    //     return nullptr;

    // }

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

            if (temp->left) {

                temp->left->value = equal1->get_value();

                for(size_t i = 0; i < variables.size(); i++){

                    if(variables.at(i)->raw_value == temp->left->raw_value){

                        variables.erase(variables.begin()+i);

                    }

                }

            }

            variables.push_back(temp->left);      

            equal = temp;

        } else {

            return equal;

        }

    }

}

// for(size_t i = 0; i < variables.size(); i++){

//                 if(variables.at(i)->raw_value == temp->left->raw_value){

//                     variables.at(i)->value = temp->left->value;

//                 }else {

//                     variables.push_back(temp->left);      

//                 }

//             }

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

            Add* temp = new Add;

            temp->left = term;

            temp->right = term1;

            term = temp;

        } else if(nextToken->raw_value == "-"){

            scanToken();

            Token* term1 = parseTerm();

            Subtract* temp = new Subtract;

            temp->left = term;

            temp->right = term1;

            term = temp;

        } else {

            return term;

        }

    } 

}


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

            Multiply* temp = new Multiply;

            temp->left = factor;

            temp->right = factor1;

            factor = temp;

        } else if(nextToken->raw_value == "/"){

            scanToken();

            Token* factor1 = parseFactor();

            Divide* temp = new Divide;

            temp->left = factor;

            temp->right = factor1;

            factor = temp;

        } else {

            return factor;

        }

    }

}


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

            // cout << "here 4";

            // cout << "Unexpected token at line 1" << " column " << nextToken->column << ": " << nextToken->raw_value << endl;

            return nullptr;

        }

        Token* expression = parseEqual();

        if (expression == nullptr) {

            // cout << "here 3";

            // cout << "Unexpected token at line 1" << " column " << nextToken->column << ": " << nextToken->raw_value << endl;

            is_vaild = false;

            return nullptr;

        }

        if(nextToken->raw_value == ")"){

            scanToken();

            return expression;

        } else {

            // cout << "here 2";

            // cout << "Unexpected token at line 1" << " column " << nextToken->column << ": " << nextToken->raw_value << endl;

            is_vaild = false;

            return nullptr;

        }

    } else {

        // cout << "here 1";

        // cout << "Unexpected token at line 1" << " column " << nextToken->column << ": " << nextToken->raw_value << endl;

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
