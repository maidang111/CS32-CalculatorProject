CC=g++ -std=c++17
CFLAGS= -Wall -Wextra -Werror
OBJS_CALC = src/lib/AST.o src/lib/Lexer.o src/calc.o src/lib/InfixParser.o src/lib/AST_Node.o src/lib/Data.o
OBJS_FORMAT = src/lib/AST.o src/lib/Lexer.o src/format.o src/lib/InfixParser.o src/lib/AST_Node.o src/lib/Data.o src/lib/Formater.o src/lib/Statement.o
OBJS = src/lib/AST.o src/lib/Lexer.o src/scrypt.o src/lib/InfixParser.o src/lib/AST_Node.o src/lib/Data.o src/lib/Scrypter.o src/lib/Statement.o
MAIN= scrypt calc format

all: $(MAIN)

scrypt: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

calc: $(OBJS_CALC)
	$(CC) $(CFLAGS) $(OBJS_CALC) -o $@

format: $(OBJS_FORMAT)
	$(CC) $(CFLAGS) $(OBJS_FORMAT) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


%.o: src/lib/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -r src/*.o src/lib/*.o