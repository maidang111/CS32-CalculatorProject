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
  
