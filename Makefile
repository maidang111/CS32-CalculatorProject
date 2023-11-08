CC=g++ -std=c++17
CFLAGS= -Wall -Wextra -Werror
OBJS = src/lib/AST.o src/lib/Lexer.o src/lib/InfixParser.o src/lib/Scrypter.o src/lib/Statement.o src/scrypt.o
MAIN= format

all: $(MAIN)

format: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


%.o: src/lib/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -r src/*.o src/lib/*.o