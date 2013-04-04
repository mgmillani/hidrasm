CC = g++
INCLUDE = ../../include
CFLAGS = -Wall -Wextra -g -I$(INCLUDE)
SRC = ../../src
OBJ = .
BIN = .
OBJECTS = $(OBJ)/stringer.o caseCmp.o
NAME = casecmp

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)/$(NAME)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f *.o
	rm -f $(NAME)