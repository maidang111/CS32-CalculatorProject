# Scrypt Project CS32
## Checkpoint 1: 
### Track A: 
#### Purpose: 
The purpose of this tract is to write a lexer that takes text input and turn it into a series of tokens for the parser to uses. The possible token types includes:
* Numbers
* /, +, *, (, )
* END token (the end token is created by the program and added at the end of the text input)

The Lexer is a class that contains the following functions:
* Lexer() (the construction prompts for the user input)
* Lexer.create_tokens() (creates a series of token and stores it in the Tokens vector)
* Lexer.print_tokens() (prints the token's index and linenumber)
* ~Lexer() (the destructor deletes any token that's in the vector)

Some examples of invaild input includes: 
* A number that ends with a decimal, has multiple decimal, a singular floating decimal
* Any character that isn't a possible token type

### Track A: 
#### Purpose: 
  
