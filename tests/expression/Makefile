CC = g++
INCLUDE = ../../include
CFLAGS = -Wall -Wextra -g -I$(INCLUDE)
SRC = ../../src
OBJ = .
BIN = .
LIB = -lboost_regex
OBJECTS = $(OBJ)/expression.o $(OBJ)/file.o $(OBJ)/stringer.o expressionTest.o
NAME = expression

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIB) -o $(BIN)/$(NAME)
	rm -f *.o

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f *.o
	rm -f $(NAME)