# Scrypt Project CS32
## Checkpoint 1: 
### Track A: 
#### Purpose: 
The purpose of this tract is to write a lexer that takes in text input and turn it into a series of tokens for the parser to uses. The possible token types includes:
* Numbers.
* / , + , * , ( , ).
* END token (the end token is created by the create_token() and added at the end of the list of tokens).

The Lexer is a class that contains the following functions:
* Lexer() (the constructor prompts for the user input).
* Lexer.create_tokens() (creates a series of tokens and stores it as a list of tokens ).
* Lexer.print_tokens() (prints the tokens' linenumber, index, and value).
* ~Lexer() (the destructor deletes every token stored in the class).

Some examples of invaild inputs includes: 
* A number that ends with a decimal, has multiple decimals, or a singular floating decimal.
* Any character that isn't a possible token type.

### Track B: 
#### Purpose: 
The purpose of this tract is to write a parser that takes in the vector of pointers for tokens that lexer already created to print and calculate numerical value. The possible token value includes:
* Numbers.
* Operators(/, *, + -)
* END token

The Parser is a class that contains the following fucntions:
* Parser() (the constructor without any parameters)
* Parser.read_tokens() (read in the vector of token pointers and build AST by creating AST nodes for each token)
* Parser.print_error_2() (print out that there was an error in during parsing, it exit the code immediately)
* Parser.calculate() (evaluate and calculate the expression)
* Parser.calculate_help() (helper function for calculate() that calculate the value for AST node recursively)
* Parser.print() (print out expression and print out the calculated value)
* Parser.print_help() (helper function for print() that output the AST node value recursively)
* ~Parser() (desctructor that calls the helper function to delete nodes) 
* Parser.delete_help() (helper function for destructor that delete the AST node recursively)

Some examples of invalid inputs includes:
* invalid parenthesis: the number of left parenthesis should always be equal or greater than that of right
* invalid parenthesis: if the last token was left parenthesis and the current is right parenthesis, it yields error
* operators should come after left parenthesis
* no input value
* division by 0
* the number of operators should not exceed the number of left parenthesis

