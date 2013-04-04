CC = g++
INCLUDE = ../../include
CFLAGS = -Wall -Wextra -g -I$(INCLUDE)
SRC = ../../src
OBJ = .
BIN = .
LIB = -lboost_regex
OBJECTS = $(OBJ)/multiExpression.o $(OBJ)/expression.o $(OBJ)/file.o $(OBJ)/stringer.o multiTest.o
NAME = mex

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