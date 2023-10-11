all: main

main: main.o Lexer.o Parser.o 
	g++ main.o Lexer.o Parser.o -o main

main.o: main.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c main.cpp	

Token.o: Token.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c Token.cpp	


Lexer.o: Lexer.cpp 
	g++ -std=c++17 -Wall -Wextra -Werror -c Lexer.cpp

Parser.o: Parser.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -c Parser.cpp


clean: 
	rm -f main.o Lexer.o Parser.o main