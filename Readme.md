## Purpose:
The purpose of this project is to build a user-friendly high-level programming language using c++. We included the basic features of computer science language. It supports various data types including numbers, null, boolean values, and arrays. These data could be stored in the form of variables. Its basic functionality involve arithmetic operations, variables, boolean operations, if/else statements, while loop, array built-in functions, user defined functions, and built-in print functions.


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
### Purpose: The purpose of this track is to require the semicolon after every statement and modify how some of operators treat errors. The edition was made when it evaluates assignment operator and equality and inequality operators.


When the left side of the assignment operator is not a variable, it is no longer treated as parsing error. Instead, it is treated as runtime error. Equality and inequality operators now supports different data types.
### Track B:
#### Purpose: The purpose of this track is to implement user defined functions. These functions consist of the def keyword in the beginning of initialization. After that, the user can call the function using the function name followed by an open parenthesis, optional values, and then followed by a closing parenthesis.


We have added a few definitions to the following classes.


AST_NODE:
	class Function_Val: (use to evaluate functions)
	Bool is_Function; (determines if a node is a function or now)


InfixParser:
	bool isFunction: (checks if the current parser body is a function)
	Bool isNull; (check if the return type is a null)
	Vector <Token*> functionName; (stores a list of tokens that are function names)
	
Scrypter:
	FunctionCall* buildFunction(); (creates a function call that’s separate from build AST)


Statement:
	Class Function:
	Vector <Token*> returnStatement; (stores list of tokens in return statement)
	Vector <Token*> parameters; (stores list of tokens in function’s parameters)
	String functionName; (stores the function’s name)
	
	Class FunctionCall:
	Vector <Token*> returnStatement; (stores list of tokens in return statement)
	Vector <Token*> parameters; (stores list of tokens in function’s parameters)
	String functionName; (stores the function’s name)


### Track C:
#### Purpose: The purpose of this track is to implement array data type. The code now supports array index accessing and modification of array through array index. Also, array built-in functions were implemented to ease the use of arrays for users.


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


Only assignment, equality and inequality, and parentheses opeartions support array data type.


InfixParser linked files:
* AST.h
* AST_Node.h
* Data.h


InfixParser is the class that stores the following functions:
* InfixParser() (It is a constructor for InfixParser. It takes in user-input)
* ~InfixParser() (it is a desctructor for InfixParser that calls delete help function)
* read_all_token() (it parse all the user-input and make into a vector of ASTs)
* check_array() (it checks if the given line is an array)
* check_array_val() (it checks if the given line is an array index value)
* check_single_array_function (it checks if the given line is an array function)
* isFucntion() (it checks if the given string is a function name)
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
* Array (node for array)
* Array_Val (node for array value in that index)
* Function_Val (node for user-defined functions)
* Array_Fct (node for array built in functions)


Data:
Data is a class that stores the value that is used for evaluation on AST_Node. It handles both cases for boolean and double.


Data has the following functions:
* Data() (Data constructor that sets default values for each member variables)
* Data() (Data constructor that has paremeters that sets member variables to themselves)
* operator<< (supports cout of the data(overload));
* operator == (supports comparison of two data objects)


AST:
AST is a class that was defined for lexer input that stores the user-input as string values
