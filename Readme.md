# Scrypt Project CS32
## Purpose: 
The purpose of this project is to build user-friendly high-level programming language using c++. We included the basic features of computer science language which involve arithmetic operations, variables, boolean operations, if/else statements, while loop, and implemented print functions. 

## And Overview of each Feature: 
Format formats the user's input into a more readable set of instructions. This is useful if the user wants to check for mistakes in their input. 
Scrypt calculates the user's input and returns a number. 
Calc takes in one line at a time and calculates, print and stores the result

## How to Execute The Program: 
*Clone the file to a local directory and open it with an IDE such as VSCode
*Run the make file using the command "make"
*To run format.cpp use the command ./format
*To run scrypt.cpp use the command ./scrypt
*To run calc.cpp use the command ./calc

### TrackA
### Purpose: The purpose of this tract is to include boolean type as well as the operations that involve boolean value to infix parser. Also, mode was included into one of its arithmetic operations. The main class that supports the following functionalities are InfixParser calss. 

### Testing for tractA:
./calc is the executable for testing this track. After inputing make followed by ./calc, a user input is necessary to run the code. The file that contains the main() is called calc.cpp. It is linked to the files that has Lexer class, AST class, and InfixParser class. 

## Classes
InfixParser:
InfixParser will be supporting the following operations from highes to lowest:
* Parentheses.
* Multiplication, division, and modulo. (arithmetic operation that returns double)
* Addition and subtraction. (arithmetic operation that returns double)
* Comparisons. (arithemetic operation that returns bool)
* Equality and inequality. (it can both evaluate either double or bool but it cannot handle different type operand case)
* Logical: and. (a boolean operation that takes in bool returns bool)
* Logical: or exclusive. (a boolean operation that takes in bool returns bool)
* Logical: or inclusive. (a boolean operation that takes in bool returns bool)
* Assignment. (it assigns left operand(variable) to value)

InfixParser linked files:
* AST.h
* AST_Node.h
* Data.h

InfixParser is the class that stores the following functions:
* InfixParser() (It is a constructor for InfixParser. It takes in user-input)
* ~InfixParser() (it is a desctructor for InfixParser that calls delete help function)
* read_all_token() (it parse all the user-input and make into a vector of ASTs)
* read_token() (It parse one line of user-input and make it into a AST)
* check_error() (It checks for parsing errors that involve parentheses, operator and literal values) 
* check_assignment() (It checks for parsing errors that involve assignment)
* check_for_statment() (It makes sure it does not have any statement such as print, if, else, and while)
* print_all(): prints out all the expression including their evaluated value
* read_one_line() (It reads one line of code and make it into a AST we can assume that there is no parsing error)
* evaluate() (It evaluate the single line of user-input)
* evaluate_print() (It prints out evaluated value)
* update_variables() (It updates the variable that was modified on the last evaluation)
* single_value_token() (It handles the line that has only single element and make it into a AST, it is a edge case for read_one_line function)

Invalid cases:
* not assigned idenitfier
* invalid assignee
* invalid parenthesis
* invalid operation (ex. operator that only handles double has bool has its operand)
* divide by zero

AST_Node:
AST_Node is a node class for AST that is built in InfixParser class. It has numerous derived classes that was built to store different values/operations. It uses the class Data to store the value of each token

AST_Nodes linked files: 
* Data.h
* AST.h

AST_Node contains:
* AST_Node pointer to the left child
* AST_Node pointer to the right child
* AST_Node pointer to parent
* Token pointer that points to its token allocated in heap
* Data class object called val

AST_Node has the following functions:
* AST_Node() (constructor with/without token pointer as parameters that initialize val)
* ~AST_Node() (destructor)
* get_val() (virtual function that evaluate the value)
* is_variable() (virtual function that checks if the function input is variable or  not)
* invalid_variable() (virtual function that determines if the variable already has assigned value)

Derived Classes of AST_Node:
* Double_Operation (node for the arithmetic operators +, -, *, /, and *)
* Direct_Val (node for direct value such as number or true/false)
* Variable_Val (node for variable)
* Assign (node for =)
* Boolean_Operation (node for logicals |, ^, &)
* Equality_Val (node for inequality and equality == , !=)
* Comparison_Val (node for comparison operators <=, <, >=, >)

Data:
Data is a class that stores the value that is used for evaluation on AST_Node. It handles both cases for boolean and double. 

Data has the following functions:
* Data() (Data constructor that sets default values for each member variables)
* Data() (Data constructor that has paremeters that sets member variables to themselves)

AST:
AST is a class that was defined for lexer input that stores the user-input as string values

Lexer:
Lexer is a class that generate user input as a vector of AST pointer variables

### Track B: 
#### Purpose: The purpose of this track is to implement if, while, and print statement that uses expressions from Track A. The If statement body executes when its conditional is true. If it is false, it executes the else statement body. The while statement repeates the body until condition becomes false. The print statement just print out the body. If, while, and print can be in each other's body.  

The class Statement has five subclasses (While, If, Else, Print, Expression). The statement class contain the following functions and variables: 
* Statement() (create a new statment block for one of the following type)
* size_t level (stores the depth of each block)
* vector <Token*> condition (stores a vector of tokens for evaulating true and false statements)
* vector <Statement*> body (stores a vector of statements that is called in that block)
* virtual ~Statement() (destructor the statement class)
* virtual void print() (prints out the format of expressions with correct grammar and indentation)
* virtual void deleteStatement() (a helper function that deletes each statement block and its childrens)
* virtual void calculate(InfixParser* infixParser) (calculates the value of the expression and updates the value for the scope of the input call)

The class Formater is used by format.cpp to print out the formated version of the user's input. The Fomater class has the following functions and variables:
* Formater(vector <Token*>) (creates a Formter class with a vector of tokens)
* ~Formater() (Destructor for Formater)
* vector <Token*> tokens (stores a vector of tokens lexed from the user's input)
* vector <Statement*> ASTHeads (store the head of every )
* size_t index (keeps track the index of the current token)
* int level (keeps track of the depth of the AST)
* void delete_tokens() (deletes tokens lexed)
* void deleteStatements() (deletes the statments for every AST in a recursive manner)
* void buildASTs() (Build all the AST expressions)
* void printFormated() (prints the formated version of user's input)
* Statement* buildAST() (builds indivisual AST child with While, If, Else, Print, Expression)

The class Formater is used by scrypt.cpp to print out the the calculated value of the user's input. The Scrypt class has the following functions and variables:
* Scrypter(vector <Token*>) (creates a Scrypter class with a vector of tokens)
* ~Scrypter() (Destructor for Scrypter)
* InfixParser* infixparser (stores the pointer to an infixeparser)
* Vector <Token*> tokens (stores a vector of tokens lexed from the user's input)
* vector <Statement*> ASTHeads (store the head of every )
* size_t index (keeps track the index of the current token)
* int level (keeps track of the depth of the AST)
* void delete_tokens() (deletes tokens lexed)
* void deleteStatements() (deletes the statments for every AST in a recursive manner)
* void buildASTs() (Build all the AST expressions)
* void calculate() (calculates the value of the user's input)
* Statement* buildAST() (builds indivisual AST child with While, If, Else, Print, Expression)
