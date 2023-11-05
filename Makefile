CC=g++ -std=c++17
CFLAGS= -Wall -Wextra -Werror
OBJS = src/lib/AST.o src/lib/Lexer.o src/lib/InfixParser.o src/scrypt.o
MAIN= scrypt

all: $(MAIN)

scrypt: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


%.o: src/lib/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -r src/*.o src/lib/*.o