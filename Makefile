CC = gcc
SRC = $(wildcard src/commands/*.c)
OBJ = $(patsubst src/commands/%.c,build/obj/%.o,$(SRC))
BIN = $(patsubst build/obj/%.o,build/bin/%,$(OBJ))
UTILS = build/obj/utils.o

all: build utils $(OBJ) $(BIN)

build:
	mkdir -p build/bin
	mkdir -p build/obj

utils:
	$(CC) -c src/include/utils.c -o $(UTILS)

build/obj/%.o: src/commands/%.c
	$(CC) -c $< -o $@

build/bin/%: build/obj/%.o
	$(CC) $< $(UTILS) -o $@

clean:
	rm -rf build