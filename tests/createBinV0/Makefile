CC = g++
INCLUDE = ../../include
CFLAGS = -Wall -Wextra -g -I$(INCLUDE)
SRC = ../../src
OBJ = .
BIN = .
OBJECTS = binv0.o $(OBJ)/assembler.o $(OBJ)/instructions.o $(OBJ)/addressings.o $(OBJ)/machine.o $(OBJ)/registers.o $(OBJ)/labels.o $(OBJ)/messenger.o $(OBJ)/stringer.o $(OBJ)/file.o $(OBJ)/memory.o $(OBJ)/numbers.o $(OBJ)/directives.o $(OBJ)/sha1.o
NAME = binv0

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