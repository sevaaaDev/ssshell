
SRC = ./src/executioner.cpp ./src/executioner_utils.cpp ./src/parser.cpp ./src/main.cpp ./src/builtins_command.hpp ./src/lexer.cpp
OBJ = ${SRC:.c=.o}

ssshell: ${OBJ}
	g++ -g -o $@ ${OBJ}
