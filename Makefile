CC=g++ -std=c++17
CFLAGS= -Wall -Wextra -Werror
OBJS =src/lex.o src/parse.o src/lib/AST.o src/lib/Lexer.o src/lib/Number.o src/lib/Node.o src/lib/Operator.o src/lib/Parser.o
MAIN= lex

all: $(MAIN)

lex: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


%.o: src/lib/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -r src/*.o src/lib/*.o