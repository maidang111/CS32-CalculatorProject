
all: lex

lex: lex.o Lexer.o parser.o AST.o
	g++ lex.o Lexer.o parser.o AST.o -o lex

lex.o: lex.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c lex.cpp	

parser.o: parser.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c parser.cpp

AST.o: AST.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c AST.cpp	

Lexer.o: Lexer.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c lib/Lexer.cpp	

clean: 
	rm -f AST.o Lexer.o parser.o Token.o lex.o 