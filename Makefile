CC = gcc

FLAGS = -Werror -Wall -Wpedantic
DEBUG_FLAGS = -ggdb -O0

LIBS = -lraylib -lm -lGL -lpthread -ldl -lrt -lX11

SRC = src/*
BIN = bin/
EXE = $(BIN)chip8

.PHONY: all debug clean

all: $(BIN)
	$(CC) $(FLAGS) -o $(EXE) $(SRC) $(LIBS)

$(BIN):
	mkdir $(BIN)

debug: $(BIN)
	$(CC) $(FLAGS) $(DEBUG_FLAGS) -o $(EXE) $(SRC) $(LIBS)

clean:
	rm -f $(EXE)