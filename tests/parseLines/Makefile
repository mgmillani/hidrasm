CC = g++
INCLUDE = ../../include
CFLAGS = -Wall -Wextra -g -I$(INCLUDE)
SRC = ../../src
OBJ = .
BIN = .
OBJECTS = $(OBJ)/assembler.o $(OBJ)/addressings.o $(OBJ)/instructions.o $(OBJ)/registers.o $(OBJ)/machine.o $(OBJ)/labels.o $(OBJ)/stringer.o $(OBJ)/memory.o $(OBJ)/file.o $(OBJ)/sha1.o parse.o
NAME = parse

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)/$(NAME)
	rm -f *.o

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f *.o
	rm -f $(NAME)