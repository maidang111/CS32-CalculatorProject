
all: lex

lex: lex.o Lexer.o parser.o AST.o
	g++ lex.o Lexer.o parser.o AST.o -o lex

lex.o: src/lex.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c src/lex.cpp	

parser.o: src/parser.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c src/parser.cpp

AST.o: src/lib/AST.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c src/lib/AST.cpp	

Lexer.o: src/lib/Lexer.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c src/lib/Lexer.cpp	

Node.o: src/lib/Node.cpp Operator.o Number.o
	g++ -std=c++17 -Wall -Wextra -Werror -c src/lib/Node.cpp	

Operator.o: src/lib/Operator.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c src/lib/Operator.cpp	

Number.o: src/lib/Number.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c src/lib/Number.cpp	




clean: 
	rm -f AST.o Lexer.o parser.o Token.o lex.o 