CC = g++
INCLUDE = ../../include
CFLAGS = -Wall -Wextra -g -I$(INCLUDE)
SRC = ../../src
OBJ = .
BIN = .
OBJECTS = $(OBJ)/messenger.o $(OBJ)/file.o $(OBJ)/stringer.o msg.o
NAME = msg

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