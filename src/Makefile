all: lex

lex: lex.o Lexer.o Parser.o  AST.o
	g++ lex.o Lexer.o Parser.o AST.o -o lex

lex.o: lex.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c lex.cpp	

AST.o: AST.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c AST.cpp	


Lexer.o: Lexer.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c Lexer.cpp

Parser.o: Parser.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c Parser.cpp


clean: 
	rm -f AST.o Lexer.o Parser.o Token.o lex.o 