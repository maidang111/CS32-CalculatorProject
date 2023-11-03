# Scrypt Project CS32
## Checkpoint 2: 
### Track A: 
#### Purpose: 
The purpose of this tract is to implement = operator and validate the use of variables during calculation. The edition of Lexer from checkpoint 1 involved recognizing = operator and variables and converting them as tokens. The edition of Parser from checkpoint 2 involved assigning the numerical value to each of variable as well as defining the invalid operation using = operator and variables. 

Lexer 
* Numbers.
* Variables
* / , + , * , ( , ), =
* END token (the end token is created by the create_token() and added at the end of the list of tokens).

The Lexer is a class that contains the following functions:
* Lexer() (the constructor prompts for the user input).
* Lexer.create_tokens() (creates a series of tokens and stores it as a list of tokens ).
* Lexer.print_tokens() (prints the tokens' linenumber, index, and value).
* ~Lexer() (the destructor deletes every token stored in the class).

Some examples of invaild inputs includes: 
* A number that ends with a decimal, has multiple decimals, or a singular floating decimal.

The Parser is a class that contains the following fucntions:
* Parser() (the constructor without any parameters)
* Parser.read_all_lines() (read in entire vector of tokens and call read_tokens fucntion when each for each expression, building AST for each of them)
* Parser.read_tokens() (read in the vector of token pointers and build AST by creating AST nodes for each token)
* Parser.print_error_2() (print out that there was an error in during parsing, it exit the code immediately)
* Parser.calculate() (evaluate and calculate the expression)
* Parser.calculate_help() (helper function for calculate() that calculate the value for AST node recursively)
* Parser.print() (print out expression and print out the calculated value)
* Parser.print_help() (helper function for print() that output the AST node value recursively)
* ~Parser() (desctructor that calls the helper function to delete nodes) 
* Parser.delete_help() (helper function for destructor that delete the AST node recursively)

Some examples of invalid inputs includes:
* after = operator, if first operand is not variable
* = operator only has one operand
* in the parenthesis that involve = operator, if double value exist, it should be the last element in that parenthesis. 
* if the double was not assigned to the variable, the calculation involve that variable except for its assignment with other value causes error
* invalid parenthesis: the number of left parenthesis should always be equal or greater than that of right
* invalid parenthesis: if the last token was left parenthesis and the current is right parenthesis, it yields error
* operators should come after left parenthesis
* no input value
* division by 0
* the number of operators should not exceed the number of left parenthesis

### Track B: 
#### Purpose: The purpose of this track is to code a calculator that takes infix notations. The calculator would have defined order of operations starting with parentheses, then multiplication, subtraction, addition, subtraction. It can also set vaulues and store them with the assignment operation. The InfixParser takes in tokens parsed from a slightly modfied Lexer from track A. 

InfixParser will be supporting the following operations from highest to lowest:
*Parentheses.
*Multiplication and division.
*Addition and subtraction.
*Assignment.

We define the calculator to be left associative except for the equal sign which will be right associative

The InfixParser is a class that contains the following functions:
*InfixParser() (calls the Lexer to lex the user's inputs into tokens)
*Token* parseExpression() (Evaluates + and - tokens)
*Token* parseFactor() (Evaluates * and / tokens)
*Token* parseTerm() (Evaluates variables, numbers and parantheses)
*Token* parseEqual() (Evaluates equal signs)
*void delete_tokens() (delete lex tokens)
*void delete_variables() (delete storied variable tokens)
*bool error_parenthesis(size_t index) (handels errors that relates to parentheses)
*bool error_assignment(size_t index) (handel assignment errors)
*void scanToken() (movies pointer to the next token in vector)
*~InfixParser() (Destructor)

Some examples of invaild inputs includes: 
*Invalid Operations (some examples includes multiple operators in a row, ending with an operator)
*Invalid Assignment (some examples includes assigning a variable to a number)
*Invalid parentheses (some examples includes open parentheses, an absent of closing parenthese, invalid expressing inside of parenthese)

