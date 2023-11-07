CC=g++ -std=c++17
CFLAGS= -Wall -Wextra -Werror
OBJS = src/lib/AST.o src/lib/Lexer.o src/lib/InfixParser.o src/lib/Formater.o src/lib/Statement.o src/calc.o
MAIN= calc

all: $(MAIN)

calc: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


%.o: src/lib/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -r src/*.o src/lib/*.o