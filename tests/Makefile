CC = g++
INCLUDE = ../include
CFLAGS = -Wall -Wextra -g -I$(INCLUDE)
SRC = ../src
OBJ = obj
BIN = .
LIB = -lboost_regex
OBJECTS = generalTest.o $(OBJ)/directives.o $(OBJ)/addressings.o $(OBJ)/registers.o $(OBJ)/instructions.o $(OBJ)/machine.o $(OBJ)/labels.o $(OBJ)/memory.o $(OBJ)/stringer.o $(OBJ)/numbers.o $(OBJ)/sha1.o $(OBJ)/messenger.o $(OBJ)/expression.o $(OBJ)/multiExpression.o $(OBJ)/assembler.o $(OBJ)/file.o $(OBJ)/operands.o $(OBJ)/types.o
NAME = generalTest

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIB) -o $(BIN)/$(NAME)
	rm -f *.o

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(OBJ)/*.o
	rm -f $(NAME)
