SRC = main.cpp parser_test.cpp ../src/parser.cpp tree_test.cpp builtincmd_test.cpp ../src/lexer.cpp lexer_test.cpp
OBJ = ${SRC:.cpp=.o}

all: test

.cpp.o:
	g++ -std=c++20 -c $< -o $@

test: ${OBJ}
	g++ -std=c++20 -o $@ ${OBJ}

clean:
	rm -f test ${OBJ}

.PHONY: all clean
